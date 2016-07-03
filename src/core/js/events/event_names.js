/**
 * This module provides all event names currently used in the application. There is an equivalent of this class in
 * c++ code of the app.
 * @module voicemynews/js/events/event_names
 */
"use strict";

module.exports = {
    /**
     * This event tells to the native part that js business logic started successfully.
     */
    APP_START: "app:js:start",

    /**
     * This event tells to the native part that js business logic shutdowned successfully.
     */
    APP_SHUTDOWN: "app:js:shutdown",

    /**
     * This event tells the business logic to load the navigation menu.
     */
    APP_NAVIGATION_MENU_LOAD: "app:navigation_menu:load",

    /**
     * This event tells the native part that the navigation menu has been loaded successfully. 
     */
    APP_NAVIGATION_MENU_LOADED: "app:navigation_menu:loaded"
};