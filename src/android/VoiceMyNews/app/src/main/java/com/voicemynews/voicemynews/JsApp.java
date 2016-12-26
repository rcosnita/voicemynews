package com.voicemynews.voicemynews;

import android.content.res.AssetManager;

import com.voicemynews.core.bindings.news.VoiceSupport;
import com.voicemynews.core.bindings.news.VoiceSupportActions;

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
     * @param nativeEmiterPtr the native emitter memory location used for the js application.
     * @param assetManager the application asset manager which can be used for accessing assets folder.
     * @param voiceSupport a voice support instance which can be used for TTS operations.
     */
    public static native void startPlatform(long nativeEmiterPtr, AssetManager assetManager,
                                            VoiceSupport voiceSupport);

    /**
     * Provides the logic for shutting down the js platform (v8 platform) and cleaning up all allocated
     * resources.
     *
     * @param {Long} the native emitter memory location used for the js application.
     */
    public static native void shutdownPlatform(long nativeEmitterPtr);
}
