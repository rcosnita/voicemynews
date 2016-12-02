package com.voicemynews.voicemynews.components;

import android.app.Activity;
import android.content.Context;
import android.content.res.TypedArray;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.util.AttributeSet;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;

import com.voicemynews.core.bindings.events.EventDataBindingNative;
import com.voicemynews.core.bindings.events.EventHandler;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.voicemynews.App;
import com.voicemynews.voicemynews.R;
import com.voicemynews.voicemynews.models.JsonArrayAdapter;
import com.voicemynews.voicemynews.models.SideMenuState;
import com.voicemynews.voicemynews.utils.JsResourcesHelper;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

/**
 * Provides the main menu implementation. This compoment, loads all the menu entries from js and
 * displays them. Moreover, it takes of persisting the state of the selected item.
 */
public class MainMenuView extends LinearLayout {
    private Activity parentActivity = null;
    private App appContext = null;
    private JsonArrayAdapter menuItemsModel = null;
    private String appTitle = null;
    private EventLoopBindingNative eventLoop;
    private DrawerLayout sideMenu = null;
    private ListView sideMenuList = null;
    private ActionBarDrawerToggle sideMenuToggle = null;

    public EventLoopBindingNative getEventLoop() {
        return eventLoop;
    }

    public void setEventLoop(EventLoopBindingNative eventLoop) {
        this.eventLoop = eventLoop;
        initSideMenu();
    }

    public ActionBarDrawerToggle getSideMenuToggle() {
        return sideMenuToggle;
    }

    public MainMenuView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context, attrs);
    }

    /**
     * Initializes the current object with custom attributes.
     */
    private void init(Context context, AttributeSet attrs) {
        appContext = (App)context.getApplicationContext();
        parentActivity = (Activity)context;

        if (attrs != null) {
            this.initAttrs(context, attrs);
        }

        inflate(context, R.layout.view_main_menu, this);
    }

    /**
     * Provides the logic for initializing all custom attributes for the current view.
     * @param ctx The context under which the view is running.
     * @param attrs All attributes received by the view.
     */
    private void initAttrs(Context ctx, AttributeSet attrs) {
        TypedArray a = ctx.getTheme().obtainStyledAttributes(
                attrs,
                R.styleable.MainMenuView,
                0, 0);

        try {
            appTitle = a.getString(R.styleable.MainMenuView_app_title);
        } finally {
            a.recycle();
        }
    }

    /**
     * Initializes the side menu of the application. It lists all items from the business logic.
     */
    private void initSideMenu() {
        final String title = appTitle;
        final String drawerTitle = title;
        sideMenu = (DrawerLayout) parentActivity.findViewById(R.id.side_menu_layout);
        sideMenuList = (ListView) findViewById(R.id.side_menu);
        sideMenuToggle = new ActionBarDrawerToggle(parentActivity, sideMenu,
                R.string.sidemenu_open, R.string.sidemenu_close) {

            public void onDrawerClosed(View view) {
                super.onDrawerClosed(view);
                parentActivity.getActionBar().setTitle(title);
                appContext.getSideMenuState().setVisible(false);
            }

            public void onDrawerOpened(View drawerView) {
                super.onDrawerOpened(drawerView);
                parentActivity.getActionBar().setTitle(drawerTitle);
                appContext.getSideMenuState().setVisible(true);
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
                post(new Runnable() {
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
            itemsResources.put("icon", new JsonArrayAdapter.PopulateViewAction() {
                @Override
                public void populate(JSONObject item, String jsonKey, View view) {
                    try {
                        ImageView iconView = (ImageView) view.findViewById(R.id.sidemenu_item_icon);
                        iconView.setImageResource(JsResourcesHelper.getIdFromDrawableResourcePath(item.getString(jsonKey), iconView.getContext()));
                    } catch(Exception ex) {
                        // TODO [rcosnita] handle exceptions correctly.
                        System.out.println(ex);
                    }

                }
            });

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

            menuItemsModel = new JsonArrayAdapter(parentActivity, R.layout.app_side_menu, menuItems,
                    itemsResources, new JsonArrayAdapter.ItemRenderedAction() {
                @Override
                public void handleSelectedItem(int position, JSONObject item, View view) {
                    SideMenuState state = appContext.getSideMenuState();

                    if (position != state.getSelectedItem()) {
                        return;
                    }

                    view.setBackgroundResource(R.color.sideMenuItemSelectedBackground);
                }
            });
            sideMenuList.setAdapter(menuItemsModel);
            sideMenuList.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    selectMenuItem(position);
                }
            });

            int selectedItem = appContext.getSideMenuState().getSelectedItem();
            if (selectedItem < 0) {
                return;
            }

            sideMenuList.setSelection(selectedItem);
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
        appContext.getSideMenuState().setSelectedItem(position);

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
