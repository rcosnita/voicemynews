package com.voicemynews.core.models;

import android.content.Context;
import android.support.annotation.LayoutRes;
import android.support.annotation.NonNull;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Map;

/**
 * Provides a simple adapter capable of displaying json arrays into a given list view.
 */
public class JsonArrayAdapter extends BaseAdapter {
    /**
     * Provides the contract which must be implemented whenever we want to populate a resource with
     * the value of a json key.
     */
    public interface PopulateViewAction {
        /**
         * The actual logic for populating a resource for the given item and key.
         *
         * @param item The item from where we can extract the actual jsonKey value.
         * @param jsonKey The json key we currently use for populating a resource.
         * @param view The view we are currently populating with data.
         */
        void populate(JSONObject item, String jsonKey, View view);
    }
    /**
     * The current context in which the adapter was used.
     */
    private Context context;

    /**
     * The itmes which back up the adapter.
     */
    private JSONArray items;

    /**
     * Provides a mapping between the key we want to display from each json array item and the resource
     * where it is going to be displayed.
     */
    private Map<String, PopulateViewAction> itemsResources;

    /**
     * Provides the identifier of the layout which we currently populate with items.
     */
    private int layoutResource;

    private LayoutInflater layoutInflater;

    public JsonArrayAdapter(
            @NonNull
            Context context,
            @LayoutRes
            int layoutResource,
            @NonNull
            JSONArray items,
            @NonNull
            Map<String, PopulateViewAction> itemsResources) {
        this.context = context;
        this.layoutResource = layoutResource;
        this.items = items;
        this.itemsResources = itemsResources;
        this.layoutInflater = LayoutInflater.from(context);
    }

    @Override
    public int getCount() {
        return items.length();
    }

    @Override
    public Object getItem(int position)  {
        try {
            return items.getJSONObject(position);
        } catch(JSONException ex) {
            // TODO [rcosnita] log exception correctly.
            return null;
        }
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        return createView(position);
    }

    /**
     * Provides a very simple implementation which retrieves a populated view for the requested
     * position. Internally, it binds every key from the given {@see itemsResources} map to the actual
     * value from the JSONObject found at the requested position.
     *
     * @param position the position of the view we are currently creating.
     * @return
     */
    private View createView(int position) {
        View view = layoutInflater.inflate(layoutResource, null);
        JSONObject obj = (JSONObject) getItem(position);
        for (String jsonKey : itemsResources.keySet()) {
            PopulateViewAction action = itemsResources.get(jsonKey);
            action.populate(obj, jsonKey, view);
        }

        return view;
    }
}
