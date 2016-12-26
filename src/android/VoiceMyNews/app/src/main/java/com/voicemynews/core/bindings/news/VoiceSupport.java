package com.voicemynews.core.bindings.news;

import android.speech.tts.TextToSpeech;

/**
 * Provides the contract for supporting voice operations within voicemynews. A concrete implementation
 * of this contract will be binded to js business logic.
 */
public interface VoiceSupport extends TextToSpeech.OnInitListener {
    /**
     * Provides the logic for reading text using a TTS engine. While reading, specific
     * callbacks are going to be invoked.
     *
     * @param text The plain text we want to read.
     * @param callbacks The callbacks which must be invoked in order to notify progress.
     */
    void readText(String text, VoiceSupportActions callbacks);

    /**
     * Provides the logic for reading text using a TTS engine. While reading, specific
     * callbacks are going to be invoked. It is desired to use this method whenever it is possible
     * in order to obtain higher quality results.
     *
     * @param text The ssml formatted text we want to read.
     * @param callbacks The callbacks which must be invoked in order to notify progress.
     */
    void readSsml(String text, VoiceSupportActions callbacks);

    /**
     * Pause the current TTS session.
     *
     * @param callbacks The callbacks used for notifying about paused action.
     */
    void pause(VoiceSupportActions callbacks);

    /**
     * Resume the current TTS session.
     *
     * @param callbacks The callbacks used for notifying about resume action.
     */
    void resume(VoiceSupportActions callbacks);

    /**
     * Skip / stop the current tts session.
     */
    void skip();
}
