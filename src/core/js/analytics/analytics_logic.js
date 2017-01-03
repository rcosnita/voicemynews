"use strict";

/**
 * This module provides analytics api which can be used for logging / tracking events. It follows the same logic as
 * described in the analytics spec.
 * @module voicemynews/js/analytics/analytics
 */

const InvalidArgumentException = require("js/exceptions/invalid_argument").InvalidArgumentException;

const SCREEN_EVENT_TYPE = 1;
const CUSTOM_EVENT_TYPE = 2;

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
     * @param {Number} evt.eventValue the event value.
     * @param {String} evt.screenName (optional) the screen name we want to track. If this is specified all other properties are optional.
     */
    logEvent(evt) {
        let evtType = CUSTOM_EVENT_TYPE;

        if (!evt) {
            throw new InvalidArgumentException("evt");
        }

        if (evt.screenName) {
            evtType = SCREEN_EVENT_TYPE;
            evt.eventCategory = evt.screenName;
            evt.eventAction = "";
            evt.eventLabel = "";
            evt.eventValue = 0;
        } else {
            if (!evt.eventCategory) {
                throw new InvalidArgumentException("evt.eventCategory");
            }

            if (!evt.eventAction) {
                throw new InvalidArgumentException("evt.eventAction");
            }

            if (!evt.eventLabel) {
                throw new InvalidArgumentException("evt.eventLabel");
            }

            if (!evt.eventValue) {
                throw new InvalidArgumentException("evt.eventValue");
            }
        }

        const evtNative = this._analyticsNative.buildEvent(CUSTOM_EVENT_TYPE, evt.eventCategory, evt.eventAction, evt.eventLabel,
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