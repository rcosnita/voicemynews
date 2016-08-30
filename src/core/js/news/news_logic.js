/**
 * This module provides the business logic for managing news operations.
 * @module voicemynews/js/news/news_logic
 */
"use strict";

const newsException = require("js/exceptions/invalidnews");
const EventNames = require("js/events/event_names");
const Q = require("js/third_party/q/q");

/**
 * Provides the news manager which implement supported operations and event handlers.
 *
 * @public
 * @class
 * @alias module:voicemynews/js/news/news_logic.NewsLogic
 */
class NewsLogic {
    /**
     * Allows developers to inject all required properties into the instance.
     *
     * @param {Object} eventLoop the event emitter loop which can be used to communicate with the native part.
     * @param {Function} buildEventData the function which can build native compatible events.
     * @param {Object} newsProviders a dictionary containing all available providers (datasources) for news. See
     * {@link module:voicemynews/js/news/news_datasource} for more information.
     */
    constructor(eventLoop, buildEventData, newsProviders) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
        this._newsProviders = newsProviders;
    }

    /**
     * Provides the main entry point for wiring all events expected to be received from native part.
     */
    init() {
        this._eventLoop.on(EventNames.NEWS_FETCH_BY_URL, (evtData) => this._handleFetchNewsByUrlEvent(evtData.evtData));
    }

    /**
     * Provides the algorithm for loading a news from the given url. Internally,
     * it uses the newsProviderId to choosed the datasource capable of loading the news.
     *
     * @param {String} url the news url from where we can load the news content using http requests.
     * @param {String} newsProviderId the datasource unique identifier which can be used to load the news.
     * @param {Object} rssDesc the rss descriptor obtained when parsing rss xml descriptor for the given news url.
     * @return {Promise} a promise which resolves to the news model for the given url or rejects with a specific exception.
     */
    fetchNewsByUrl(url, newsProviderId, rssDesc) {
        let loader = Q.defer();
        const datasource = this._newsProviders[newsProviderId];

        if(!datasource) {
            throw new newsException.NewsProviderNotFoundException(undefined, newsProviderId);
        }

        datasource.fetchNews(url, rssDesc).then((newsModel) => {
            loader.resolve(newsModel);
        }, (rejectionResult) => {
            loader.reject(rejectionResult);
        });

        return loader.promise;
    }

    /**
     * Provides the logic for handling fetch news events received from native part. Once the news are fetched,
     * a news by url loaded event is emitted.
     */
    _handleFetchNewsByUrlEvent(evtDataStr) {
        let evtData = JSON.parse(evtDataStr);
        const url = evtData.url;
        const providerId = evtData.provider;

        this.fetchNewsByUrl(url, providerId, evtData).then((newsModel) => {
            const loadedEvt = this._buildEventData(JSON.stringify(newsModel));
            this._eventLoop.emit(EventNames.NEWS_FETCH_BY_URL_LOADED, loadedEvt);
        }, (rejectionResult) => {
            const rejectionResultStr = this._buildEventData(JSON.stringify(rejectionResult));
            this._eventLoop.emit(EventNames.NEWS_FETCH_BY_URL_LOADED, rejectionResultStr);
        });
    }
}

module.exports = {
    NewsLogic: NewsLogic,
    init: (eventLoop, buildEventData, newsProviders) => {
        const newsLogic = new NewsLogic(eventLoop, buildEventData, newsProviders);
        newsLogic.init();
        return newsLogic;
    }
}