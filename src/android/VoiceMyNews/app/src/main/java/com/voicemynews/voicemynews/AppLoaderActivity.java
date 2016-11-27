package com.voicemynews.voicemynews;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class AppLoaderActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText("Initializing app");
    }
}
