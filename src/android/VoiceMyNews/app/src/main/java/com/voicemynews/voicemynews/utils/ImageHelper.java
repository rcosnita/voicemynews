package com.voicemynews.voicemynews.utils;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

/**
 * Provides helper methods for scaling / and efficiently decoding images for the android platform.
 */
public class ImageHelper {
    /**
     * Decode image efficiently taking in account the desired width used for display.
     * @return The new scaled bitmap which can be displayed.
     */
    public static Bitmap decodeSampledBitmapFromResource(Resources res, int resId,
                                                         int reqWidth) {

        final BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;
        BitmapFactory.decodeResource(res, resId, options);

        int outWidth = options.outWidth * (int)res.getDisplayMetrics().density;
        int sampleSize = reqWidth < outWidth ? outWidth / reqWidth : reqWidth / outWidth;
        options.inSampleSize = sampleSize;

        options.inJustDecodeBounds = false;
        return BitmapFactory.decodeResource(res, resId, options);
    }
}
