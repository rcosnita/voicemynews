(function() {

/**
 * @class
 * Provides the js code which can log events to the analytics backend. When it is instantiated,
 * it starts listening for post messages.
 */
function AnalyticsApp(wnd, allowedEventNames, getTrackingBackend) {
    this._wnd = wnd;
    this._allowedEventNames = allowedEventNames || [];
    this._getTrackingBackend = getTrackingBackend;
    this._listen();

    console.log("Analytics layer started ...");
}

AnalyticsApp.prototype._listen = function() {
    var self = this;

    this._wnd.addEventListener("message", function(evt) {
        self._handleMessage(JSON.parse(evt.data));
    }, false);
}

AnalyticsApp.prototype._handleMessage = function(evt) {
    if (this._allowedEventNames.indexOf(evt["custom-eventName"]) < 0) {
        return;
    }

    this._getTrackingBackend()("send", "event", evt);
}

return new AnalyticsApp(window, ["js:analytics:log:event"], function() { return ga; });

})();