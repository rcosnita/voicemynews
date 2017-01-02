package com.voicemynews.voicemynews.utils;

import com.voicemynews.voicemynews.AppLoaderActivity;

/**
 * Provide helper methods for working with tasks / multithreads on Android platform.
 */
public class Tasks {
    /**
     * Provides a helper method for running an action on the ui thread. We are currently using a
     * pointer to AppLoaderActivity instance in order to get an ui thread.
     * @param action The action we want to run on the ui thread.
     */
    public static void runOnUiThread(Runnable action) {
        AppLoaderActivity.getCurrentInstance().runOnUiThread(action);
    }
}
