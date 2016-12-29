"use strict";

/**
 * @class
 * Provides the js code which can log events to the analytics backend. When it is instantiated,
 * it starts listening for post messages.
 */
class AnalyticsApp {
    constructor(wnd, allowedEventNames, trackingBackend) {
        this._wnd = wnd;
        this._allowedEventNames = allowedEventNames || [];
        this._trackingBackend = trackingBackend;
        this._listen();

        console.log("Analytics layer started ...");
    }

    _listen() {
        this._wnd.addEventListener("message", (evt) => this._handleMessage(JSON.parse(evt.data)), false);
    }

    _handleMessage(evt) {
        if (this._allowedEventNames.indexOf(evt["custom-eventName"]) < 0) {
            return;
        }

        console.log(evt);
        this._trackingBackend("send", "event", evt.eventCategory, evt.eventAction, evt.eventLabel, evt.eventValue);
    }
}

new AnalyticsApp(window, ["js:analytics:log:event"], ga);