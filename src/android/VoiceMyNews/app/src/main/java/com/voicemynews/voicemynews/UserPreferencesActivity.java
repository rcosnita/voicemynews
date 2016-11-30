package com.voicemynews.voicemynews;

import android.app.FragmentManager;
import android.os.Bundle;

import com.voicemynews.voicemynews.components.CategoriesPreferencesFragment;

/**
 * Provides the categories preferences activity. This allows user to configure his interests.
 */
public class UserPreferencesActivity extends MainPageAppActivity {
    public UserPreferencesActivity() {
        super(R.layout.activity_categories_preferences);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        CategoriesPreferencesFragment fragment = CategoriesPreferencesFragment.newInstance();

        FragmentManager fragmentManager = getFragmentManager();
        fragmentManager.beginTransaction()
                .replace(R.id.content_frame, fragment)
                .commit();
    }
}
