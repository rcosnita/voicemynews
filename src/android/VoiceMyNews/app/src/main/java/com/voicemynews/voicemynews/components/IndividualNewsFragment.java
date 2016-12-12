package com.voicemynews.voicemynews.components;

import android.app.Fragment;
import android.content.Context;
import android.graphics.Typeface;
import android.os.Bundle;
import android.util.TypedValue;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.TextView;

import com.voicemynews.core.bindings.events.EventDataBindingNative;
import com.voicemynews.core.bindings.events.EventHandler;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.voicemynews.App;
import com.voicemynews.voicemynews.IndividualNewsActivity;
import com.voicemynews.voicemynews.R;
import com.voicemynews.voicemynews.models.JsonArrayIterator;

import org.json.JSONArray;
import org.json.JSONObject;

/**
 * Provides the fragment which can display individual news.
 */
public class IndividualNewsFragment extends Fragment {
    private EventLoopBindingNative eventLoop = null;
    private JSONObject newsModel = null;
    private String newsModelStr = null;
    private JSONObject articleModel = null;
    private TextView headlineView = null;
    private ViewGroup contributedByLst = null;
    private ViewGroup paragraphsView = null;
    private View currView = null;

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

        this.newsModelStr = getArguments().getString(IndividualNewsActivity.PROP_NEWS_MODEL);

        try {
            newsModel = new JSONObject(this.newsModelStr);
        } catch (Exception ex) {
            // TODO [rcosnita] handle exception correctly.
            System.err.println(ex);
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        super.onCreateView(inflater, container, savedInstanceState);

        currView = inflater.inflate(R.layout.fragment_individual_news, container, false);
        currView.findViewById(R.id.btnReadNews).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onReadNewsClick(v);
            }
        });
        headlineView = (TextView) currView.findViewById(R.id.individual_news_headline);
        contributedByLst = (ViewGroup) currView.findViewById(R.id.individual_news_contributed_by);
        paragraphsView = (ViewGroup) currView.findViewById(R.id.individual_news_paragraphs);

        wireJsModel(newsModelStr);

        return currView;
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
    }

    @Override
    public void onDetach() {
        super.onDetach();
    }

    public void onReadNewsClick(View view) {
        eventLoop.emit("js:news:voice:read", EventDataBindingNative.getInstanceNative(articleModel.toString()));
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
        final String headline = articleModel.getString("headline");
        final JSONArray contributedByArr = articleModel.getJSONArray("contributedBy");
        final JSONArray paragraphsArr = articleModel.getJSONArray("paragraphs");

        headlineView.setText(headline);
        displayContributedBy(contributedByArr);
        displayParagraphs(paragraphsArr);
    }

    /**
     * Provides the logic for displaying contributed by section of the article into the current layout.
     */
    private void displayContributedBy(final JSONArray contributedByArr) throws Exception {
        final LayoutInflater inflater = LayoutInflater.from(contributedByLst.getContext());

        new JsonArrayIterator(contributedByArr, JsonArrayIterator.JsonItemType.STRING).forEach(new JsonArrayIterator.JsonArrayIteratorOnItem() {
            @Override
            public void onItemReceived(Object obj) {
                final String label = obj.toString();
                final View view = inflater.inflate(R.layout.fragment_individual_news_contributed_by,
                        contributedByLst);
                ((TextView) view.findViewById(R.id.individual_news_contributed_by_label)).setText(label);
            }
        });
    }

    /**
     * Provides the logic for displaying the paragraphs section of the article into the current layout.
     */
    private void displayParagraphs(final JSONArray paragraphsArr) throws Exception {
        final int[] subheadings = getResources().getIntArray(R.array.news_subheadings_font_size);
        final LayoutInflater inflater = LayoutInflater.from(paragraphsView.getContext());

        new JsonArrayIterator(paragraphsArr, JsonArrayIterator.JsonItemType.OBJECT).forEach(new JsonArrayIterator.JsonArrayIteratorOnItem() {
            @Override
            public void onItemReceived(Object obj) {
                try {

                    inflater.inflate(R.layout.fragment_individual_news_paragraphs,
                            paragraphsView);
                    TextView contentView = (TextView) ((FrameLayout) paragraphsView.getChildAt(paragraphsView.getChildCount() - 1)).findViewById(R.id.individual_news_paragraphs_content);

                    JSONObject item = (JSONObject) obj;
                    final int headingLevel = item.getInt("subheadingLevel");
                    contentView.setText(item.getString("content"));

                    if (headingLevel > 0) {
                        int fontSize = subheadings[headingLevel - 1];
                        contentView.setTextSize(TypedValue.COMPLEX_UNIT_SP, fontSize);
                        contentView.setTypeface(null, Typeface.BOLD);
                    }
                } catch (Exception ex) {
                    // TODO [rcosnita] handle exception correctly.
                    System.err.println(ex);
                }
            }
        });
    }
}
