package com.voicemynews.voicemynews;

import android.app.Activity;
import android.content.res.Configuration;
import android.os.Bundle;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v4.widget.DrawerLayout;
import android.view.MenuItem;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import com.voicemynews.core.bindings.events.EventLoopBindingNative;

/**
 * This activity displays the main ui of the application and takes decision of what to display.
 * In case this is the first launch of the app, user will be guided through settings. Otherwise,
 * the user will see his news.
 */
public class AppStartActivity extends Activity {
    private DrawerLayout sideMenu = null;
    private ListView sideMenuList = null;
    private EventLoopBindingNative eventLoop = null;
    private ActionBarDrawerToggle sideMenuToggle = null;

    /**
     * Provides the creation logic for the application main activity.
     * @param savedInstanceState The saved state from a previous run session.
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_app_start);

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
     * Provides the logc handling the clicks on the menus.
     * @param item The item selected from the menu.
     * @return
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (sideMenuToggle.onOptionsItemSelected(item)) {
            return true;
        }

        // TODO [rcosnita] implement the app menu in here.

        return super.onOptionsItemSelected(item);
    }

    /**
     * Initializes the side menu of the application. It lists all items from the business logic.
     */
    private void initSideMenu() {
        final String mTitle = getTitle().toString();
        final String mDrawerTitle = mTitle;
        String[] titles = {"menu item 1", "menu item 2"};
        sideMenu = (DrawerLayout) findViewById(R.id.drawer_layout);
        sideMenuList = (ListView) findViewById(R.id.left_drawer);
        sideMenuList.setAdapter(new ArrayAdapter<>(this, R.layout.app_side_menu, R.id.drawer_text, titles));
        sideMenuToggle = new ActionBarDrawerToggle(this, sideMenu,
                R.string.drawer_open, R.string.drawer_close) {

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
    }

    /**
     * Just a test method for making sure menu loading works as expected.
     */
    private void testMenuLoading() {
//        eventLoop.on("app:navigation_menu:loaded", new EventHandler() {
//            @Override
//            public void handleEvent(final EventDataBindingNative evtData) {
//                runOnUiThread(new Runnable() {
//                    @Override
//                    public void run() {
//                    tvWelcome.setText(evtData.getEvtData());
//                    }
//                });
//            }
//        });
//
//        eventLoop.emit("app:navigation_menu:load", EventDataBindingNative.getInstanceNative(""));
    }
}
