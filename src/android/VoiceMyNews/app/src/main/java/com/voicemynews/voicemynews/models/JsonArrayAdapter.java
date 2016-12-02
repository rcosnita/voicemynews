package com.voicemynews.voicemynews.models;

import android.content.Context;
import android.support.annotation.LayoutRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Map;

/**
 * Provides a simple adapter capable of displaying json arrays into a given list view.
 */
public class JsonArrayAdapter extends BaseAdapter {
    /**
     * Provides a data model which holds the result of a get operation where compoud key is specified.
     * Compound keys are used to navigate multiple hierarchy levels of a json object.
     */
    private static class CompoundKeyResult {
        public static final char COMPOUND_KEY_SEPARATOR = '.';
        public final String key;
        public final JSONObject jsonObject;

        public CompoundKeyResult(String key, JSONObject jsonObject) {
            this.key = key;
            this.jsonObject = jsonObject;
        }

        /**
         * Tells if the given key is compound key or not.
         * @param key A json key we intend to use for accessing a property of a json object.
         * @return
         */
        public static boolean isCompoundKey(String key) {
            return key.indexOf('.') >= 0;
        }
    }

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
     * Provides the contract which must be implemented in case the consumer of jsonarrayadapter wants
     * to take actions whenever the a new item is being rendered.
     */
    public interface ItemRenderedAction {
        /**
         * The actual logic for handling the item being rendered.
         *
         * @param position The current position of the item being rendered.
         * @param item The current item data.
         * @param view The view associated with the current item.
         */
        void handleSelectedItem(int position, JSONObject item, View view);
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
     * Provides an action which is invoked whenever the selected item is rendered. In case no such
     * action is provided this step is skipped.
     */
    private ItemRenderedAction onItemRendered = null;

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
        this(context, layoutResource, items, itemsResources, null);
    }

    public JsonArrayAdapter(
            @NonNull
            Context context,
            @LayoutRes
            int layoutResource,
            @NonNull
            JSONArray items,
            @NonNull
            Map<String, PopulateViewAction> itemsResources,
            @Nullable
            ItemRenderedAction onItemRendered) {
        this.context = context;
        this.layoutResource = layoutResource;
        this.items = items;
        this.itemsResources = itemsResources;
        this.layoutInflater = LayoutInflater.from(context);
        this.onItemRendered = onItemRendered;
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
     * @return The newly created view with all resources populated.
     */
    private View createView(int position) {
        View view = layoutInflater.inflate(layoutResource, null);
        JSONObject obj = (JSONObject) getItem(position);

        try {
            for (String jsonKey : itemsResources.keySet()) {
                JSONObject tmpObj = obj;
                PopulateViewAction action = itemsResources.get(jsonKey);

                if (CompoundKeyResult.isCompoundKey(jsonKey)) {
                    do {
                        int dotPos = jsonKey.indexOf(CompoundKeyResult.COMPOUND_KEY_SEPARATOR);
                        String currKey = jsonKey.substring(0, dotPos);
                        jsonKey = jsonKey.substring(dotPos + 1);
                        tmpObj = tmpObj.getJSONObject(currKey);

                    } while (CompoundKeyResult.isCompoundKey(jsonKey));
                }

                action.populate(tmpObj, jsonKey, view);
            }

            if (onItemRendered != null) {
                onItemRendered.handleSelectedItem(position, obj, view);
            }
        } catch (Exception ex) {
            // TODO [rcosnita] handle exception correctly.
            System.out.println(ex);
        }

        return view;
    }
}
