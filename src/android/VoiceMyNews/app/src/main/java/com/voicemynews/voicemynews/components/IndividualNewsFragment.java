package com.voicemynews.voicemynews.components;

import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.voicemynews.core.bindings.events.EventDataBindingNative;
import com.voicemynews.core.bindings.events.EventHandler;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.voicemynews.App;
import com.voicemynews.voicemynews.IndividualNewsActivity;
import com.voicemynews.voicemynews.R;

import org.json.JSONObject;

/**
 * Provides the fragment which can display individual news.
 */
public class IndividualNewsFragment extends Fragment {
    private EventLoopBindingNative eventLoop = null;
    private JSONObject newsModel = null;
    private JSONObject articleModel = null;
    private TextView headlineView = null;

    public IndividualNewsFragment() { }

    public static IndividualNewsFragment newInstance(String newsModel) {
        IndividualNewsFragment fragment = new IndividualNewsFragment();
        Bundle args = new Bundle();
        args.putString(IndividualNewsActivity.PROP_NEWS_MODEL, newsModel);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        eventLoop = ((App) getActivity().getApplicationContext()).getEventLoop();

        String newsModelStr = getArguments().getString(IndividualNewsActivity.PROP_NEWS_MODEL);

        try {
            newsModel = new JSONObject(newsModelStr);
        } catch (Exception ex) {
            // TODO [rcosnita] handle exception correctly.
            System.err.println(ex);
        }

        wireJsModel(newsModelStr);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_genius_news, container, false);
        headlineView = (TextView) view.findViewById(R.id.individual_news_headline);

        return view;
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
    }

    @Override
    public void onDetach() {
        super.onDetach();
    }

    /**
     * Provides the logic for loading the individual news from business logic. It passes the newsModelStr
     * as event data.
     * @param newsModelStr The selected news model string representation.
     */
    private void wireJsModel(String newsModelStr) {
        eventLoop.on("js:news:get:byurl:loaded", new EventHandler() {
            @Override
            public void handleEvent(EventDataBindingNative evtData) {
                try {
                    String article = evtData.getEvtData();
                    articleModel = new JSONObject(article);

                    getActivity().runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                displayNews();
                            } catch (Exception ex) {
                                // TODO [rcosnita] handle the exception.
                                System.err.println(ex);
                            }
                        }
                    });
                } catch (Exception ex) {
                    // TODO [rcosnita] handle the exception.
                    System.err.println(ex);
                }
            }
        });

        eventLoop.emit("js:news:get:byurl", EventDataBindingNative.getInstanceNative(newsModelStr));
    }

    /**
     * Displays the current news by populating the view with specific information. All paragraphs are
     * added as dynamic fragments into article content list view.
     */
    private void displayNews() throws Exception {
        String headline = articleModel.getString("headline");

        headlineView.setText(headline);
    }
}
