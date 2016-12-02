package com.voicemynews.voicemynews;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import com.voicemynews.core.bindings.events.EventDataBindingNative;
import com.voicemynews.core.bindings.events.EventHandler;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;

public class AppLoaderActivity extends Activity {
    private String appStartKey = null;
    private EventLoopBindingNative eventLoop;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        eventLoop = ((App)getApplicationContext()).getEventLoop();

        appStartKey = eventLoop.on("app:js:start", new EventHandler() {
            @Override
            public void handleEvent(EventDataBindingNative evtData) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    displayMainUI();
                }
            });
            }
        });
    }

    /**
     * Navigate to main page of the application.
     */
    private void displayMainUI() {
        Context appCtx = getApplicationContext();
        Intent mainUiIntent = new Intent(this, AppStartActivity.class);
        mainUiIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        appCtx.startActivity(mainUiIntent);
    }
}
