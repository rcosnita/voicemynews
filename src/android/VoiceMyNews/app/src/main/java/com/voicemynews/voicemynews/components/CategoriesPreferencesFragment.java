package com.voicemynews.voicemynews.components;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.voicemynews.voicemynews.R;

/**
 * Provides the fragment capable of allowing users to configure their categories preferences.
 */
public class CategoriesPreferencesFragment extends Fragment {
    public CategoriesPreferencesFragment() { }

    public static CategoriesPreferencesFragment newInstance() {
        CategoriesPreferencesFragment fragment = new CategoriesPreferencesFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_categories_preferences, container, false);
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
    }

    @Override
    public void onDetach() {
        super.onDetach();
    }
}
