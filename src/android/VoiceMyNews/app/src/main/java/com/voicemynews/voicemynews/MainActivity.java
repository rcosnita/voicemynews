package com.voicemynews.voicemynews;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

import com.voicemynews.core.bindings.events.EventLoopBinding;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("voicemynews-core");
        System.loadLibrary("voicemynews-android");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText("Hello world ...");

        EventLoopBinding eventLoop = EventLoopBindingNative.getInstance();
    }
}
