package com.voicemynews.voicemynews;

import android.app.FragmentManager;
import android.os.Bundle;

import com.voicemynews.voicemynews.components.GeniusNewsFragment;

/**
 * Provides the activity which displays all news available for the current user.
 */
public class GeniusNewsActivity extends MainPageAppActivity {
    public GeniusNewsActivity() {
        super(R.layout.activity_genius_news);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        GeniusNewsFragment fragment = GeniusNewsFragment.newInstance();

        FragmentManager fragmentManager = getFragmentManager();
        fragmentManager.beginTransaction()
                .replace(R.id.content_frame, fragment)
                .commit();
    }
}
