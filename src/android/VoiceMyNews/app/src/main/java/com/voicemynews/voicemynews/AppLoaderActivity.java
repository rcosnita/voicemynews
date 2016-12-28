package com.voicemynews.voicemynews;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import com.voicemynews.core.bindings.analytics.Analytics;
import com.voicemynews.core.bindings.events.EventDataBindingNative;

public class AppLoaderActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ((App)getApplicationContext()).setAppStartedListener(new App.OnAppStarted() {
            @Override
            public void doAction(EventDataBindingNative evtData) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        App.getCurrent().setAnalytics(Analytics.startAnalytics(getApplicationContext()));
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
        App appCtx = (App)getApplicationContext();
        appCtx.clearAppStartedListeners();
        Intent mainUiIntent = new Intent(this, AppStartActivity.class);
        mainUiIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        appCtx.startActivity(mainUiIntent);
    }
}
