package com.voicemynews.voicemynews;

import android.app.Activity;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.support.annotation.LayoutRes;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.TextView;

import com.voicemynews.core.bindings.events.EventDataBindingNative;
import com.voicemynews.core.bindings.events.EventHandler;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.voicemynews.models.JsonArrayAdapter;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

/**
 * Provides the foundation activity which can be extended by each app main page activity. It provides
 * the boilerplate of populating the side menu and providing access to the event loop which can be
 * used to communicate with app business logic.
 */
public abstract class MainPageAppActivity extends Activity {
    private JsonArrayAdapter menuItemsModel = null;
    private EventLoopBindingNative eventLoop = null;
    private int layoutId;
    private DrawerLayout sideMenu = null;
    private ListView sideMenuList = null;
    private ActionBarDrawerToggle sideMenuToggle = null;

    public MainPageAppActivity(@LayoutRes int layoutId) {
        this.layoutId = layoutId;
    }

    /**
     * Retrieves the current event loop instance which can be used to communicate with js business
     * logic.
     * @return
     */
    protected EventLoopBindingNative getEventLoop() {
        return eventLoop;
    }

    /**
     * Retrieves the menu items model for the side menu used during navigation.
     * @return
     */
    protected JsonArrayAdapter getMenuItemsModel() {
        return menuItemsModel;
    }

    /**
     * Provides the creation logic for the application main activity.
     * @param savedInstanceState The saved state from a previous run session.
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(layoutId);

        eventLoop = ((App)getApplicationContext()).getEventLoop();

        initSideMenu();

        getActionBar().setDisplayHomeAsUpEnabled(true);
        getActionBar().setHomeButtonEnabled(true);
    }

    /**
     * In case the configuration change, update all relevant pieces of UI.
     * @param newConfig the newly received configuration.
     */
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        sideMenuToggle.onConfigurationChanged(newConfig);
    }

    /**
     * Provides the logic for correctly syncing the state from a previous running instance.
     * @param savedInstanceState The previous saved instance.
     */
    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        sideMenuToggle.syncState();
    }

    /**
     * Provides the logic handling the clicks on the menus.
     * @param item The item selected from the menu.
     * @return
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (sideMenuToggle.onOptionsItemSelected(item)) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    /**
     * Initializes the side menu of the application. It lists all items from the business logic.
     */
    private void initSideMenu() {
        final String mTitle = getTitle().toString();
        final String mDrawerTitle = mTitle;
        sideMenu = (DrawerLayout) findViewById(R.id.drawer_layout);
        sideMenuList = (ListView) findViewById(R.id.side_menu);
        sideMenuToggle = new ActionBarDrawerToggle(this, sideMenu,
                R.string.sidemenu_open, R.string.sidemenu_close) {

            public void onDrawerClosed(View view) {
                super.onDrawerClosed(view);
                getActionBar().setTitle(mTitle);
            }

            public void onDrawerOpened(View drawerView) {
                super.onDrawerOpened(drawerView);
                getActionBar().setTitle(mDrawerTitle);
            }
        };

        sideMenu.addDrawerListener(sideMenuToggle);
        wireJsMenu();
    }

    /**
     * Just a test method for making sure menu loading works as expected.
     */
    private void wireJsMenu() {
        eventLoop.on("app:navigation_menu:loaded", new EventHandler() {
            @Override
            public void handleEvent(final EventDataBindingNative evtData) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        displayMenuItems(evtData);
                    }
                });
            }
        });

        eventLoop.emit("app:navigation_menu:load", EventDataBindingNative.getInstanceNative(""));
    }

    /**
     * Display the menu items described by the event received as input.
     * @param evtData the event describing the menu.
     */
    private void displayMenuItems(EventDataBindingNative evtData) {
        try {
            JSONArray menuItems = new JSONArray(evtData.getEvtData());

            Map<String, JsonArrayAdapter.PopulateViewAction> itemsResources = new HashMap<>();
            itemsResources.put("label", new JsonArrayAdapter.PopulateViewAction() {
                @Override
                public void populate(JSONObject item, String jsonKey, View view) {
                    TextView labelView = (TextView) view.findViewById(R.id.sidemenu_item_label);

                    try {
                        labelView.setText(item.getString(jsonKey));
                    } catch (JSONException ex) {
                        // TODO [rcosnita] handle exception correctly and log it.
                        return;
                    }
                }
            });

            menuItemsModel = new JsonArrayAdapter(this, R.layout.app_side_menu, menuItems,
                    itemsResources);
            sideMenuList.setAdapter(menuItemsModel);
            sideMenuList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    selectMenuItem(position);
                }
            });
        } catch(Exception ex) {
            // TODO [rcosnita] handle exceptions in a coherent way.
            System.out.println(ex);
        }
    }

    /**
     * Provides the logic for handling items selected from the side menu.
     * @param position The position of the item selected.
     */
    private void selectMenuItem(int position) {
        JSONObject item = (JSONObject)menuItemsModel.getItem(position);

        try {
            String evtName = item.getJSONObject("data").getString("evtName");
            String evtData = item.toString();
            eventLoop.emit(evtName, EventDataBindingNative.getInstanceNative(evtData));
        } catch (JSONException ex) {
            // TODO [rcosnita] handle and log the exception.
            System.out.println(ex);
        }
    }
}
