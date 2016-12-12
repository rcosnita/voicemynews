package com.voicemynews.core.bindings.news;

/**
 * Provides a wrapper over native actions which can be injected into voice operations.
 */
public class VoiceSupportActions {
    private long voiceReadingNotificationsPtr;

    public VoiceSupportActions(long voiceReadingNotificationsPtr) {
        this.voiceReadingNotificationsPtr = voiceReadingNotificationsPtr;
    }

    /**
     * Provides the logic for invoking the native whenPlayheadChanged callback with the
     * current position.
     * @param pos The current playhead position.
     */
    public void onPlayheadChanged(int pos) {
        invokeWhenPlayheadChangedFn(pos, voiceReadingNotificationsPtr);
    }

    /**
     * Provides the logic for invoking the native whenPaused callback with the current
     * position.
     * @param pos The current position in stream.
     */
    public void onPaused(long pos) {
        invokeWhenPausedFn(pos, voiceReadingNotificationsPtr);
    }

    /**
     * Provides the logic for invoking the native whenResumed callback with the current
     * position.
     * @param pos The current position in stream.
     */
    public void onResumed(long pos) {
        invokeWhenResumedFn(pos, voiceReadingNotificationsPtr);
    }

    /**
     * Provides the logic for invoking the native whenDone callback.
     */
    public void onDone() {
        invokeWhenDoneFn(voiceReadingNotificationsPtr);
    }

    private native void invokeWhenPlayheadChangedFn(int pos, long fnPtr);

    private native void invokeWhenPausedFn(long pos, long fnPtr);

    private native void invokeWhenResumedFn(long pos, long fnPtr);

    private native void invokeWhenDoneFn(long fnPtr);
}
