/**
 * This module provides the business logic for managing news operations.
 * @module voicemynews/js/news/news_logic
 */
"use strict";

const EventNames = require("js/events/event_names");

/**
 * Provides the news manager which implement supported operations and event handlers.
 *
 * @public
 * @class
 * @alias module:voicemynews/js/news/news_logic.NewsLogic
 */
class NewsLogic {
    constructor(eventLoop, buildEventData) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
    }

    init() { }
}

module.exports = {
    NewsLogic: NewsLogic,
    init: (eventLoop, buildEventData) => {
        const newsLogic = new NewsLogic(eventLoop, buildEventData);
        newsLogic.init();
        return newsLogic;
    }
}