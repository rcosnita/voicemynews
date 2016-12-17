package com.voicemynews.voicemynews.components;

import android.app.Activity;
import android.app.Fragment;
import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import com.voicemynews.core.bindings.events.EventDataBindingNative;
import com.voicemynews.core.bindings.events.EventHandler;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.voicemynews.App;
import com.voicemynews.voicemynews.IndividualNewsActivity;
import com.voicemynews.voicemynews.R;
import com.voicemynews.voicemynews.models.JsonArrayAdapter;
import com.voicemynews.voicemynews.utils.DownloadImageAsync;

import org.json.JSONArray;
import org.json.JSONObject;

import java.net.URL;
import java.util.HashMap;
import java.util.Map;

/**
 * Provides the fragment which can display all genius news for the current user.
 */
public class GeniusNewsFragment extends Fragment implements AdapterView.OnItemClickListener {
    /**
     * Gives access to the event loop capable of communicating with js business logic.
     */
    private EventLoopBindingNative eventLoop = null;

    /**
     * Gives access to the parent activity where this fragment is used.
     */
    private Activity parentActivity = null;

    /**
     * Provides access to the instance of list view which is going to display all available news.
     */
    private ListView newsListing = null;

    /**
     * Provides the news model for the current fragment. It might be changed as we refresh news.
     */
    private JsonArrayAdapter newsModel = null;

    public GeniusNewsFragment() { }

    public static GeniusNewsFragment newInstance() {
        GeniusNewsFragment fragment = new GeniusNewsFragment();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        parentActivity = getActivity();
        eventLoop = ((App)parentActivity.getApplicationContext()).getEventLoop();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_genius_news, container, false);
        newsListing = (ListView) view.findViewById(R.id.genius_news_listing);
        newsListing.setOnItemClickListener(this);
        wireJsModel(view);
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
     * Provides an implementation which handles news selected event. It triggers navigation to
     * individual news activity.
     */
    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        if (newsModel == null) {
            return;
        }

        JSONObject individualNewsModel = (JSONObject) newsModel.getItem(position);
        Intent individualNews = new Intent(parentActivity, IndividualNewsActivity.class);
        individualNews.putExtra(IndividualNewsActivity.PROP_NEWS_MODEL, individualNewsModel.toString());

        parentActivity.startActivity(individualNews);
    }

    /**
     * Wire all click events for every button found in the genius news layout.
     *
     * @param view the current view rendered for the fragment.
     * @param items the json array holding all news currently displayed in the playlist.
     */
    private void wireButtonEvents(View view, final JSONArray items) {
        Button btnRead = (Button)view.findViewById(R.id.btnReadNews);
        Button btnSkip = (Button)view.findViewById(R.id.btnSkipNews);
        Button btnPause = (Button)view.findViewById(R.id.btnPauseNews);
        Button btnResume = (Button)view.findViewById(R.id.btnResumeNews);

        JSONObject evtData = new JSONObject();

        try {
            evtData.put("news", items);

        } catch (Exception ex) {
            // TODO [rcosnita] correctly handle the json exception.
            System.err.print(ex);
        }

        final EventDataBindingNative evt =
                EventDataBindingNative.getInstanceNative(evtData.toString());

        wireButtonSendEvent(btnRead, "js:news:voice:read:playlist", evt);
        wireButtonSendEvent(btnSkip, "js:news:voice:read:playlist:skip", evt);
        wireButtonSendEvent(btnPause, "js:news:voice:read:playlist:pause", evt);
        wireButtonSendEvent(btnResume, "js:news:voice:read:playlist:resume", evt);
    }

    /**
     * Wires a click action which simply sends the event name with provided evt data through the eventLoop.
     */
    private void wireButtonSendEvent(Button btn, final String evtName, final EventDataBindingNative evt) {
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                eventLoop.emit(evtName, evt);
            }
        });
    }

    /**
     * Sends an event to js business logic in order to trigger genius news loading.
     */
    private void wireJsModel(final View view) {
        eventLoop.on("js:news:get:from_preferred_categories:loaded", new EventHandler() {
            @Override
            public void handleEvent(EventDataBindingNative evtData) {
            try {
                JSONArray items = new JSONArray(evtData.getEvtData());
                displayNews(view, items);
            } catch (Exception ex) {
                // TODO [rcosnita] handle the exception.
                System.out.println(ex);
            }
            }
        });

        eventLoop.emit("js:news:get:from_preferred_categories", EventDataBindingNative.getInstanceNative(""));
    }

    /**
     * Provides the logic for displaying all available news.
     *
     * @param view the view being rendered.
     * @param items The items we want to list.
     */
    private void displayNews(View view, JSONArray items) {
        Map<String, JsonArrayAdapter.PopulateViewAction> itemsResources = new HashMap<>();

        itemsResources.put("headline", new JsonArrayAdapter.PopulateViewAction() {
            @Override
            public void populate(BaseAdapter adapter, JSONObject item, String jsonKey, View view) {
                try {
                    TextView headline = (TextView) view.findViewById(R.id.genius_news_listing_headline);
                    headline.setText(item.getString(jsonKey));
                } catch (Exception ex) {
                    // TODO [rcosnita] handle the exception correctly.
                    System.out.println(ex);
                }
            }
        });

        itemsResources.put("images.small.url", new JsonArrayAdapter.PopulateViewAction() {
            @Override
            public void populate(BaseAdapter adapter, JSONObject item, String jsonKey, View view) {
                try {
                    final ImageView icon = (ImageView) view.findViewById(R.id.genius_news_listing_icon);
                    URL iconUri = new URL(item.getString(jsonKey));

                    new DownloadImageAsync(new DownloadImageAsync.DownloadNotifier() {
                        @Override
                        public void onDownloadComplete(Drawable drawable) {
                            icon.setImageDrawable(drawable);
                        }
                    }).execute(iconUri);
                } catch (Exception ex) {
                    // TODO [rcosnita] handle the exception correctly.
                    System.out.println(ex);
                }
            }
        });

        newsModel = new JsonArrayAdapter(parentActivity.getApplicationContext(),
                R.layout.genius_news_listing, items, itemsResources);
        newsListing.setAdapter(newsModel);

        wireButtonEvents(view, items);
    }
}
