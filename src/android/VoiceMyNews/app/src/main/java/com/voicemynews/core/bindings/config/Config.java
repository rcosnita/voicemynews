package com.voicemynews.core.bindings.config;

/**
 * Provides a wrapper over native app configuration. Each configuration item from the native config
 * is made available in this class.
 */
public class Config {
    private static String WEB_BASE_URL = null;

    private static String ANALYTICS_GA_TRACKING_CODE = null;

    public static String getWebBaseUrl() {
        return WEB_BASE_URL;
    }

    public static String getAnalyticsGaTrackingCode() {
        return ANALYTICS_GA_TRACKING_CODE;
    }

    /**
     * Initializes the configuration members with native values extracted from the builtin native
     * configuration namespace.
     */
    private static native void initConfig();

    static {
        initConfig();
    }
}
