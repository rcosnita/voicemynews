package com.voicemynews.voicemynews.components;

import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.voicemynews.voicemynews.R;

/**
 * Provides the fragment which can display individual news.
 */
public class IndividualNewsFragment extends Fragment {
    public IndividualNewsFragment() { }

    public static IndividualNewsFragment newInstance() {
        IndividualNewsFragment fragment = new IndividualNewsFragment();
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
        return inflater.inflate(R.layout.fragment_genius_news, container, false);
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
