/**
 * This module provides the module for navigating to various sections of the app.
 * @module voicemynews/js/events/navigation
 */
"use strict";
const NavigationBinding = voicemynews.app.win10.bindings.events.NavigationBinding.getInstance();
const buildEventData = voicemynews.core.events.EventLoopPlatform.buildEvent;

/**
 * This function provide the main method for navigating between various sections of the application.
 *
 * @param {String} evtName an event name which is uniquely mapped on a section of the app.
 * @param {JSON} evtData an optional string object which adds additional information about navigation.
 * @returns No result is returned by this function.
 */
let navigateTo = (evtName, evtData) => {
    evtData = evtData || {};
    NavigationBinding.navigateByEvent(evtName, buildEventData(JSON.stringify(evtData)));
}

module.exports = {
    navigateTo: navigateTo
}