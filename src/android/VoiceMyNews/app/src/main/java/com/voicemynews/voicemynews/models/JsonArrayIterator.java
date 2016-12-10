package com.voicemynews.voicemynews.models;

import org.json.JSONArray;

/**
 * Provides a simple iterator over json arrays which allows developer to easily execute actions. It
 * is useful whenever it is necessary to populate layouts starting from raw arrays.
 */
public class JsonArrayIterator {
    /**
     * Provides an enum which holds all possible items we currently support iterating over.
     * Based on the specified item type, the iterator will try to cast every item of the jsonarray
     * into a specific type.
     */
    public enum JsonItemType {
        UNKNOWN,
        STRING,
        OBJECT
    };

    /**
     * Provides a contract which allows consumers to be notified about new available items found
     * during iteration.
     */
    public interface JsonArrayIteratorOnItem {
        void onItemReceived(Object obj);
    }

    private JSONArray model;
    private JsonItemType itemType;

    public JsonArrayIterator(JSONArray model, JsonItemType itemType) {
        this.model = model;
        this.itemType = itemType;
    }

    /**
     * Iterates over every item belonging to the model and invokes the action.
     *
     * @param action The action we want to use for handling every individual item.
     * @throws Exception Might be raised in case there are some JSON errors.
     */
    public void forEach(JsonArrayIteratorOnItem action) throws Exception {
        for (int idx = 0; idx < model.length(); idx++) {
            Object obj = null;

            if (itemType == JsonItemType.STRING) {
                obj = model.getString(idx);
            } else if (itemType == JsonItemType.OBJECT) {
                obj = model.getJSONObject(idx);
            }

            action.onItemReceived(obj);
        }
    }
}
