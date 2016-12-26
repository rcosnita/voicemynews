package com.voicemynews.voicemynews.utils;

import android.graphics.drawable.Drawable;
import android.os.AsyncTask;

import java.net.URL;

/**
 * Provides a helper for download an image asynchronously in the background and once it is complete
 * pass it to an injected callback.
 */
public class DownloadImageAsync extends AsyncTask<URL, Integer, Drawable> {
    /**
     * Provides a model for allowing developers to inject their own callbacks for being notified
     * about various stages of the download action.
     */
    public interface DownloadNotifier {
        /**
         * This method is executed when a drawable download is complete.
         * @param drawable
         */
        void onDownloadComplete(Drawable drawable);
    }

    private final DownloadNotifier downloadNotifier;

    public DownloadImageAsync(DownloadNotifier downloadNotifier) {
        this.downloadNotifier = downloadNotifier;
    }

    @Override
    protected Drawable doInBackground(URL... params) {
        Drawable result = null;
        try {
            URL drawableUrl = params[0];
            result = Drawable.createFromStream(drawableUrl.openStream(), null);
        } catch (Exception ex) {
            // TODO [rcosnita] handle the exception correctly.
            System.out.println(ex);
        }

        return result;
    }

    @Override
    protected void onPostExecute(Drawable drawable) {
        downloadNotifier.onDownloadComplete(drawable);
    }
}
