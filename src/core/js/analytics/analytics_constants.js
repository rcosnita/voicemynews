"use strict";

/**
 * Provides all relevant constants used in analytics of voicemynews. All constants are kepts in sync with the C++
 * side.
 * @module voicemynews/js/analytics/analytics_constants
 */
module.exports = {
    categories: {
        APP_LIFECYCLE: "app_cycle"
    },
    events: {
        APP_START_ACTION: "start-native",
        JS_START_ACTION: "start-js"
    },
    labels: {
        APP_START_LABEL: "Native business logic started"
        JS_START_LABEL: "JS Business Logic Started"
    }
}