package com.voicemynews.voicemynews;

import android.app.Activity;
import android.content.res.Configuration;
import android.os.Bundle;
import android.support.annotation.LayoutRes;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.view.MenuItem;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.voicemynews.components.MainMenuView;

/**
 * Provides the foundation activity which can be extended by each app main page activity. It provides
 * the boilerplate of populating the side menu and providing access to the event loop which can be
 * used to communicate with app business logic.
 */
public abstract class MainPageAppActivity extends Activity {
    private EventLoopBindingNative eventLoop = null;
    private MainMenuView mainMenu = null;
    private DrawerLayout sideMenu = null;
    private int layoutId;

    public MainPageAppActivity(@LayoutRes int layoutId) {
        this.layoutId = layoutId;
    }

    /**
     * Provides the logic handling the clicks on the menus.
     * @param item The item selected from the menu.
     * @return
     */
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (mainMenu.getSideMenuToggle().onOptionsItemSelected(item)) {
            return true;
        }

        return super.onOptionsItemSelected(item);
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
     * Provides the creation logic for the application main activity.
     * @param savedInstanceState The saved state from a previous run session.
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(layoutId);

        eventLoop = ((App)getApplicationContext()).getEventLoop();
        sideMenu = (DrawerLayout) findViewById(R.id.side_menu_layout);
        mainMenu = (MainMenuView) findViewById(R.id.main_menu);
        mainMenu.setEventLoop(eventLoop);

        getActionBar().setDisplayHomeAsUpEnabled(true);
        getActionBar().setHomeButtonEnabled(true);

        displaySideMenuIfNecessary();
    }

    /**
     * Whenever the activity is resumed, we try to display / hide the side menu based on its global
     * state. We also try to maintain the selection from side menu.
     */
    @Override
    protected void onResume() {
        super.onResume();
        displaySideMenuIfNecessary();
    }

    /**
     * In case the configuration change, update all relevant pieces of UI.
     * @param newConfig the newly received configuration.
     */
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        mainMenu.getSideMenuToggle().onConfigurationChanged(newConfig);
    }

    /**
     * Provides the logic for correctly syncing the state from a previous running instance.
     * @param savedInstanceState The previous saved instance.
     */
    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);
        mainMenu.getSideMenuToggle().syncState();
    }

    /**
     * Provides the logic for displaying / hidding the menu if necessary. It uses the global side
     * menu state in order to take this decision.
     */
    protected void displaySideMenuIfNecessary() {
        boolean isSideMenuVisible = ((App)getApplicationContext()).getSideMenuState().isVisible();
        if (isSideMenuVisible) {
            sideMenu.openDrawer(GravityCompat.START);
        } else {
            sideMenu.closeDrawer(GravityCompat.START);
        }
    }
}
