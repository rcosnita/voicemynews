"use strict";

/**
 * This module provides analytics api which can be used for logging / tracking events. It follows the same logic as
 * described in the analytics spec.
 * @module voicemynews/js/analytics/analytics
 */

/**
 * Provides the implementation for the analytics layer. From js code, we can track various events using the same
 * code as in the native / ui side.
 *
 * @class
 * @alias module:voicemynews/js/analytics/analytics
 */
class Analytics {
    constructor() {
        this._analyticsNative = voicemynews.core.analytics.AnalyticsFactory.getInstance();
    }

    /**
     * Log the event received as argument. The event is converted into a native object and passed to the native side.
     *
     * @public
     * @method
     * @param {Object} evt the analytics event we want to build.
     * @param {String} evt.eventCategory the event category.
     * @param {String} evt.eventAction the event action.
     * @param {String} evt.eventLabel the event label.
     * @param {String} evt.eventValue the event value.
     */
    logEvent(evt) {
        const evtNative = this._analyticsNative.buildEvent(evt.eventCategory, evt.eventAction, evt.eventLabel,
            evt.eventValue);
        this._analyticsNative.logEvent(evtNative);
    }
}

module.exports = {
    init: () => {
        return new Analytics();
    },
    Analytics: Analytics
}