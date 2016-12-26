package com.voicemynews.core.bindings.news;

import android.media.MediaPlayer;
import android.os.Environment;
import android.speech.tts.TextToSpeech;
import android.speech.tts.UtteranceProgressListener;

import com.voicemynews.voicemynews.App;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Locale;
import java.util.Queue;

/**
 * Provides the voice support service implementation which uses Android TTS. This is the cheapest
 * solution but it does not necessary guarantees a high quality output.
 */
public class VoiceSupportAndroid implements VoiceSupport {
    /**
     * Provides a contract which allows us to queue actions which are going to be executed
     * when the tts engine is correctly initialized. In rare cases, we might end up trying to execute
     * TTS actions (e.g speak) while the engine is not completely initialized.
     */
    private interface ExecuteWhenReadyAction {
        void execute() throws Exception;
    }

    private static final String StreamName = App.getCurrent().getApplicationInfo().dataDir + "/temp_tts_voicemynews.wav";
    private static final VoiceSupport Instance = new VoiceSupportAndroid();

    private boolean initialized = false;
    private MediaPlayer mediaPlayer = null;
    private TextToSpeech ttsEngine;
    private Queue<ExecuteWhenReadyAction> pendingActions;

    private VoiceSupportAndroid() {
        mediaPlayer = new MediaPlayer();
        ttsEngine = new TextToSpeech(App.getCurrent(), this);
        pendingActions = new LinkedList<>();
    }

    /**
     * Provides a factory method which allows us to obtain an instance of VoiceSupport. By design,
     * voice support is a singleton object.
     * @return The instance of voice support which can be used for tts operations.
     */
    public static VoiceSupport getInstance() {
        return Instance;
    }

    @Override
    public void readText(final String text, final VoiceSupportActions callbacks) {
        final ExecuteWhenReadyAction action = new ExecuteWhenReadyAction() {
            @Override
            public void execute() throws Exception {
                mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
                    @Override
                    public void onCompletion(MediaPlayer mp) {
                        callbacks.onDone();
                        mediaPlayer.reset();
                    }
                });

                HashMap<String, String> ttsParams = new HashMap<>();
                ttsParams.put(TextToSpeech.Engine.KEY_PARAM_UTTERANCE_ID, text);

                ttsEngine.setOnUtteranceProgressListener(new UtteranceProgressListener() {
                    @Override
                    public void onStart(String utteranceId) {
                        // TODO [rcosnita] handle start synthesize event correctly.
                        System.out.println("Starting to synthesize the text ...");
                    }

                    @Override
                    public void onDone(String utteranceId) {
                        try {
                            if (!utteranceId.equals(text)) {
                                return;
                            }

                            mediaPlayer.setDataSource(StreamName);
                            mediaPlayer.prepare();
                            mediaPlayer.start();
                            callbacks.onPlayheadChanged(0);
                        } catch (Exception ex) {
                            System.err.println(ex);
                        }
                    }

                    @Override
                    public void onError(String utteranceId) {
                        if (utteranceId != text) {
                            return;
                        }

                        // TODO [rcosnita] handle error synthesize event correctly.
                        System.err.println("Error synthesizing text ....");
                    }
                });

                if (ttsEngine.synthesizeToFile(text, ttsParams, StreamName) == TextToSpeech.ERROR) {
                    // TODO [rcosnita] handle synthesize to file error handler.
                    System.err.println("Unable to synthesize " + StreamName);
                }
            }
        };

        executeOrEnqueue(action);
    }

    @Override
    public void readSsml(String text, VoiceSupportActions callbacks) {
        // TODO [rcosnita] implement this when necessary.
        throw new RuntimeException("Not implemented yet ...");
    }

    @Override
    public void pause(final VoiceSupportActions callbacks) {
        final ExecuteWhenReadyAction action = new ExecuteWhenReadyAction() {
            @Override
            public void execute() throws Exception {
                mediaPlayer.pause();
                callbacks.onPaused(mediaPlayer.getCurrentPosition());
            }
        };

        executeOrEnqueue(action);
    }

    @Override
    public void resume(final VoiceSupportActions callbacks) {
        final ExecuteWhenReadyAction action = new ExecuteWhenReadyAction() {
            @Override
            public void execute() throws Exception {
                mediaPlayer.start();
                callbacks.onResumed(mediaPlayer.getCurrentPosition());
            }
        };

        executeOrEnqueue(action);
    }

    @Override
    public void skip() {
        final ExecuteWhenReadyAction action = new ExecuteWhenReadyAction() {
            @Override
            public void execute() throws Exception {
                mediaPlayer.stop();
                mediaPlayer.reset();
            }
        };

        executeOrEnqueue(action);
    }

    @Override
    public void onInit(int status) {
        ttsEngine.setLanguage(Locale.US);
        initialized = true;

        executedPendingActions();
    }

    /**
     * Execute all pending actions in the order in which they were added.
     */
    private void executedPendingActions() {
        ExecuteWhenReadyAction action = null;

        try {
            while ((action = pendingActions.peek()) != null) {
                action.execute();
                pendingActions.remove();
            }
        } catch (Exception ex) {
            // TODO [rcosnita] handle exceptions correctly.
            System.err.println(ex);
        }
    }

    /**
     * Executes the action received as argument only if the tts engine is initialized. Otherwise,
     * it enqueues the action as a pending action.
     */
    private void executeOrEnqueue(ExecuteWhenReadyAction action) {
        try {
            if (initialized) {
                action.execute();
                return;
            }

            pendingActions.add(action);
        } catch (Exception ex) {
            // TODO [rcosnita] handle exceptions correctly.
            System.err.println(ex);
        }
    }
}
