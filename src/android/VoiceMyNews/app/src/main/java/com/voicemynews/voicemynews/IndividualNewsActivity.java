package com.voicemynews.voicemynews;

import android.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.voicemynews.voicemynews.components.GeniusNewsFragment;
import com.voicemynews.voicemynews.components.IndividualNewsFragment;

public class IndividualNewsActivity extends MainPageAppActivity {
    public IndividualNewsActivity() {
        super(R.layout.activity_individual_news);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        IndividualNewsFragment fragment = IndividualNewsFragment.newInstance();

        FragmentManager fragmentManager = getFragmentManager();
        fragmentManager.beginTransaction()
                .replace(R.id.content_frame, fragment)
                .commit();
    }
}
