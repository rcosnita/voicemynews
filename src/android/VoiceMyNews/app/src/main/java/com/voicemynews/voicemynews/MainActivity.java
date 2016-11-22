package com.voicemynews.voicemynews;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.widget.TextView;

import com.voicemynews.core.bindings.events.EventLoopBinding;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.core.bindings.network.HttpClientBinding;
import com.voicemynews.voicemynews.JsApp;

public class MainActivity extends Activity {
    private static EventLoopBinding eventLoop = null;

    static {
        System.loadLibrary("voicemynews-core");
        System.loadLibrary("voicemynews-android");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initAppJsLogic();

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText("Hello world ...");

//        EventLoopBinding eventLoop = EventLoopBindingNative.getInstance();
//        String handlerKey = eventLoop.on("custom:evt", new EventHandler() {
//            @Override
//            public void handleEvent(EventDataBindingNative evtData) {
//                System.out.println(evtData.toString());
//            }
//        });
//
//        eventLoop.emit("custom:evt", EventDataBindingNative.getInstanceNative("hello world"));
//        eventLoop.emit("custom:evt", EventDataBindingNative.getInstanceNative("hello world"));
    }

    private void initAppJsLogic()
    {
        if (eventLoop == null) {
            eventLoop = EventLoopBindingNative.getInstance();
            HttpClientBinding.initializeNative(new HttpClientBinding());
            JsApp.startPlatform(((EventLoopBindingNative) eventLoop).getNativeEmitterPtr(),
                    this.getAssets());
        }
    }
}
