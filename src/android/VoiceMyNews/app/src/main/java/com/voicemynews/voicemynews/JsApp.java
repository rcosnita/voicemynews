package com.voicemynews.voicemynews;

import android.content.res.AssetManager;

/**
 * Provides the js application bootstrap / shutdown logic. All the methods are implemented on client
 * side.
 */
public class JsApp
{
    /**
     * Provides the logic for starting js application platform (v8 engine) which is capable to
     * execute the business logic.
     *
     * @param {Long} the native emitter memory location used for the js application.
     */
    public static native void startPlatform(long nativeEmiterPtr, AssetManager assetManager);

    /**
     * Provides the logic for shutting down the js platform (v8 platform) and cleaning up all allocated
     * resources.
     *
     * @param {Long} the native emitter memory location used for the js application.
     */
    public static native void shutdownPlatform(long nativeEmitterPtr);
}
