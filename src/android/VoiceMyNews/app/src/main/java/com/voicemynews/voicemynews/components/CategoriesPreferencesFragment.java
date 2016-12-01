package com.voicemynews.voicemynews.components;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import com.voicemynews.core.bindings.events.EventDataBindingNative;
import com.voicemynews.core.bindings.events.EventHandler;
import com.voicemynews.core.bindings.events.EventLoopBindingNative;
import com.voicemynews.voicemynews.App;
import com.voicemynews.voicemynews.R;
import com.voicemynews.voicemynews.models.JsonArrayAdapter;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

/**
 * Provides the fragment capable of allowing users to configure their categories preferences.
 */
public class CategoriesPreferencesFragment extends Fragment {
    private App app;
    private EventLoopBindingNative eventLoop = null;
    private GridView categoriesView = null;
    private String onCategoriesLoaded = null;

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

        app = App.getCurrent();
        eventLoop = app.getEventLoop();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_categories_preferences, container, false);
        categoriesView = (GridView)view.findViewById(R.id.categoriesView);

        wireJsModel();

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
     * Provides the logic for loading all available categories from js business logic.
     */
    private void wireJsModel() {
        onCategoriesLoaded = eventLoop.on("js:categories:get:loaded", new EventHandler() {
            @Override
            public void handleEvent(EventDataBindingNative evtData) {
                final String data = evtData.getEvtData();

                getActivity().runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        try {
                            displayCategories(new JSONArray(data));
                        } catch (Exception ex) {
                            // TODO [rcosnita] handle the exception correctly.
                            System.out.println(ex);
                        }
                    }
                });
            }
        });

        eventLoop.emit("js:categories:get", EventDataBindingNative.getInstanceNative(""));
    }

    /**
     * Provides the logic for display categories from where user can choos his interests.
     * @param categories
     */
    private void displayCategories(JSONArray categories) {
        Map<String, JsonArrayAdapter.PopulateViewAction> itemsResources = new HashMap<>();

        itemsResources.put("name", new JsonArrayAdapter.PopulateViewAction() {
            @Override
            public void populate(JSONObject item, String jsonKey, View view) {
                TextView nameView = (TextView) view.findViewById(R.id.categoriesTitle);

                try {
                    nameView.setText(item.getString(jsonKey));
                } catch (Exception ex) {
                    // TODO [rcosnita] handle the exception correctly.
                    System.out.println(ex);
                }
            }
        });

        itemsResources.put("icon", new JsonArrayAdapter.PopulateViewAction() {
            @Override
            public void populate(JSONObject item, String jsonKey, View view) {
                try {
                    String imageSrc = item.getString(jsonKey).substring(1).toLowerCase().replace('-', '_')
                            .replace('/', '_').split("\\.")[0];
                    ImageView imageView = (ImageView) view.findViewById(R.id.categoriesImage);
                    Context imageCtx = imageView.getContext();
                    int imageId = imageCtx.getResources().getIdentifier(imageSrc, "drawable", imageCtx.getPackageName());
                    imageView.setImageResource(imageId);
                } catch (Exception ex) {
                    // TODO [rcosnita] handle the exception correctly.
                    System.out.println(ex);
                }
            }
        });

        JsonArrayAdapter categoriesModel = new JsonArrayAdapter(app, R.layout.fragment_categories_listing_column, categories,
                itemsResources);
        categoriesView.setAdapter(categoriesModel);
    }
}
