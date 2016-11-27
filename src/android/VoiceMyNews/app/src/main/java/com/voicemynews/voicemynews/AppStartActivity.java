package com.voicemynews.voicemynews;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

/**
 * This activity displays the main ui of the application and takes decision of what to display.
 * In case this is the first launch of the app, user will be guided through settings. Otherwise,
 * the user will see his news.
 */
public class AppStartActivity extends Activity {
    private TextView tvWelcome = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_app_start);

        tvWelcome = (TextView) findViewById(R.id.welcome_text);
        tvWelcome.setText("Welcome to voicemynews app ...");
    }
}
