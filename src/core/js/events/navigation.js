/**
 * This module provides the module for navigating to various sections of the app.
 * @module voicemynews/js/events/navigation
 */
"use strict";
/**
 * This class provides the logic for navigating between various sections of the app.
 * @alias module:voicemynews/js/events/navigation.NavigationManager 
 */
class NavigationManager {
    constructor(navigationBinding, buildEventData) {
        this._navigationBinding = navigationBinding || voicemynews.core.events.NavigationManagerPlatform.getInstance();
        this._buildEventData = buildEventData;
    }

    /**
     * This method provides the logic for navigating between various sections of the app.
     *
     * @public
     * @method
     * @param {String} evtName an event name which is uniquely mapped on a section of the app.
     * @param {JSON} evtData an optional string object which adds additional information about navigation.
     * @returns No result is returned by this function.
     */
    navigateTo(evtName, evtData) {
        evtData = evtData || {};
        let navigationData = this._buildEventData(JSON.stringify(evtData));
        this._navigationBinding.navigateByEvent(evtName, navigationData);
    }
}

module.exports = {
    NavigationManager: NavigationManager
}