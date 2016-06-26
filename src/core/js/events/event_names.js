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
    APP_SHUTDOWN: "app:js:shutdown"
};