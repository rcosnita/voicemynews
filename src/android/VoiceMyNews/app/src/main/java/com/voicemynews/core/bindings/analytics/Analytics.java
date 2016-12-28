package com.voicemynews.core.bindings.analytics;

import android.content.Context;

/**
 * Provides the logic for managing the analytics layer for the application.
 */
public class Analytics {
    private WebBrowserWrapper webBrowser;
    private Context ctx;

    private Analytics(Context ctx) {
        this.ctx = ctx;
    }

    /**
     * Provides the implementation for returning the code which loads the web app for analytics layer.
     */
    private native String getAnalyticsApp();

    /**
     * Provides the glue layer for notifying the native part about analytics layer start event.
     */
    private native void initAnalyticsNative();

    /**
     * Starts the analytics engine by bootstraping the desired analytics html app.
     */
    public static Analytics startAnalytics(final Context ctx) {
        final Analytics analytics = new Analytics(ctx);

        analytics.webBrowser = new WebBrowserWrapper(ctx);
        String analyticsApp = analytics.getAnalyticsApp();
        analytics.webBrowser.loadContent(analyticsApp, "text/html");
        analytics.initAnalyticsNative();

        return analytics;
    }
}
