package com.voicemynews.core.bindings.events;

import android.content.Intent;

import com.voicemynews.voicemynews.App;
import com.voicemynews.voicemynews.GeniusNewsActivity;
import com.voicemynews.voicemynews.UserPreferencesActivity;

import java.util.HashMap;
import java.util.Map;

/**
 * Provides the methods required to navigate between app activities based on input messages.
 */
public class NavigationBinding {
    private static final Map<String, Class> EVT_NAME_INTENTS = new HashMap<>();

    static {
        EVT_NAME_INTENTS.put("js:menuitems:openPreferences", UserPreferencesActivity.class);
        EVT_NAME_INTENTS.put("js:menuitems:openGenius", GeniusNewsActivity.class);
    }

    private static NavigationBinding INSTANCE = new NavigationBinding();
    private NavigationBinding() { }

    /**
     * Provides the getter for obtaining a valid instance of navigation binding.
     * @return
     */
    public static NavigationBinding getInstance() {
        return INSTANCE;
    }

    /**
     * Provides the method for navigating to the correct intent based on the given event name.
     *
     * @param evtName The event name used to uniquely identify then intent.
     * @param evtData The event data which must be passed to the intent.
     */
    public void navigateByEvent(String evtName, String evtData) {
        App app = App.getCurrent();
        Class intentType = EVT_NAME_INTENTS.get(evtName);
        Intent navigation = new Intent(app, intentType);
        navigation.putExtra("EVENT_DATA", evtData);
        app.startActivity(navigation);
    }
}
