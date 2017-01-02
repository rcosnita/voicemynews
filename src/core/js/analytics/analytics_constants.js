"use strict";

/**
 * Provides all relevant constants used in analytics of voicemynews. All constants are kepts in sync with the C++
 * side.
 * @module voicemynews/js/analytics/analytics_constants
 */
module.exports = {
    categories: {
        APP_LIFECYCLE: "app cycle",
        JS_GENIUSNEWS_ONSCREEN: "genius news onscreen",
        JS_INDIVIDUALNEWS_ONSCREEN: "individual news onscreen"
    },
    events: {
        APP_START_ACTION: "start-native",
        JS_START_ACTION: "start-js",
        JS_READ_ACTION: "read-js",
        JS_PAUSE_ACTION: "pause-js",
        JS_RESUME_ACTION: "resume-js",
        JS_SKIP_ACTION: "skip-js"
    },
    labels: {
        APP_START_LABEL: "Native business logic started",
        JS_START_LABEL: "JS Business Logic Started",
        JS_READ_LABEL: "JS Business Logic Read News",
        JS_PAUSE_LABEL: "JS Business Logic Pause News",
        JS_RESUME_LABEL: "JS Business Logic Resume News",
        JS_SKIP_LABEL: "JS Business Logic Skip News"
    }
}