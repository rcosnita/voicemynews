package com.voicemynews.voicemynews;

import android.app.FragmentManager;
import android.os.Bundle;

import com.voicemynews.voicemynews.components.IndividualNewsFragment;

public class IndividualNewsActivity extends MainPageAppActivity {

    /**
     * This property must be passed by each intent targetting IndividualNewsActivity. It tells which
     * the model of the news.
     */
    public final static String PROP_NEWS_MODEL = "PROP_INDIVIDUAL_NEWS_MODEL";

    public IndividualNewsActivity() {
        super(R.layout.activity_individual_news);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Bundle extras = getIntent().getExtras();
        String newsModelStr = extras.getString(IndividualNewsActivity.PROP_NEWS_MODEL);

        IndividualNewsFragment fragment = IndividualNewsFragment.newInstance(newsModelStr);

        FragmentManager fragmentManager = getFragmentManager();
        fragmentManager.beginTransaction()
                .replace(R.id.content_frame, fragment)
                .commit();
    }
}
