package com.voicemynews.voicemynews.utils;

import android.content.Context;

/**
 * Provides a set of helper methods meant to handle js resources links. It applies transformations
 * so that the resources can be correctly located under android app apk.
 */
public class JsResourcesHelper {
    /**
     * Provides the logic for normalizing a path from relative js URI to android correct name.
     *
     * @param path The path received from js business logic.
     * @return The normalized path for the android platform.
     */
    private static String normalizePath(String path) {
        return path.substring(1).toLowerCase().replace('-', '_')
                .replace('/', '_').split("\\.")[0];
    }

    /**
     * Obtains a drawable id from the given path.
     * @param path The path received from js business logic.
     * @param ctx The context where this method is invoked.
     * @return The unique drawable identifier.
     */
    public static int getIdFromDrawableResourcePath(String path, Context ctx) {
        path = normalizePath(path);
        return ctx.getResources().getIdentifier(path, "drawable", ctx.getPackageName());
    }
}
