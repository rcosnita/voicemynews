package com.voicemynews.voicemynews;


import android.app.Application;
import android.content.Context;
import android.content.Intent;

import com.voicemynews.core.bindings.events.EventDataBindingNative;
import com.voicemynews.core.bindings.events.EventHandler;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.core.bindings.network.HttpClientBinding;

/**
 * Provides an app implementation which allows us to initialize the native platform and bootstrap
 * v8 engine only once.
 */
public class App extends Application {
    private EventLoopBindingNative eventLoop = null;
    private String appStartKey = null;
    private Thread v8Thread = null;

    static {
        System.loadLibrary("voicemynews-core");
        System.loadLibrary("voicemynews-android");
    }

    public EventLoopBindingNative getEventLoop() {
        return eventLoop;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        initAppJsLogic();

        appStartKey = eventLoop.on("app:js:start", new EventHandler() {
            @Override
            public void handleEvent(EventDataBindingNative evtData) {
                displayMainUI();
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

    /**
     * Provides the logic for bootstraping v8 engine and voicemynews business logic.
     */
    private void initAppJsLogic()
    {
        if (eventLoop != null) {
            return;
        }

        eventLoop = (EventLoopBindingNative)EventLoopBindingNative.getInstance();

        v8Thread = new Thread(new Runnable() {
            @Override
            public void run() {
                HttpClientBinding.initializeNative(new HttpClientBinding());
                JsApp.startPlatform(eventLoop.getNativeEmitterPtr(), getAssets());
            }
        });
        v8Thread.start();
    }
}
