package com.voicemynews.voicemynews;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

import com.voicemynews.core.bindings.events.EventDataBindingNative;
import com.voicemynews.core.bindings.events.EventHandler;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;

/**
 * This activity displays the main ui of the application and takes decision of what to display.
 * In case this is the first launch of the app, user will be guided through settings. Otherwise,
 * the user will see his news.
 */
public class AppStartActivity extends Activity {
    private EventLoopBindingNative eventLoop = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_app_start);

        eventLoop = ((App)getApplicationContext()).getEventLoop();
    }

    /**
     * Just a test method for making sure menu loading works as expected.
     */
    private void testMenuLoading() {
//        eventLoop.on("app:navigation_menu:loaded", new EventHandler() {
//            @Override
//            public void handleEvent(final EventDataBindingNative evtData) {
//                runOnUiThread(new Runnable() {
//                    @Override
//                    public void run() {
//                    tvWelcome.setText(evtData.getEvtData());
//                    }
//                });
//            }
//        });
//
//        eventLoop.emit("app:navigation_menu:load", EventDataBindingNative.getInstanceNative(""));
    }
}
