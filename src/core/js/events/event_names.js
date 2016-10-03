/**
 * This module provides all event names currently used in the application. There is an equivalent of this class in
 * c++ code of the app.
 * @module voicemynews/js/events/event_names
 */
"use strict";

module.exports = Object.freeze({
    /**
     * This event tells to the native part that js business logic started successfully.
     * @constant
     */
    APP_START: "app:js:start",

    /**
     * This event tells to the native part that js business logic shutdowned successfully.
     * @constant
     */
    APP_SHUTDOWN: "app:js:shutdown",

    /**
     * This event tells the business logic to load the navigation menu.
     * @constant
     */
    APP_NAVIGATION_MENU_LOAD: "app:navigation_menu:load",

    /**
     * This event tells the native part that the navigation menu has been loaded successfully. 
     * @constant
     */
    APP_NAVIGATION_MENU_LOADED: "app:navigation_menu:loaded",

    /**
     * This event is emitted by native part and tells the business logic to obtain the available categories.
     */
    CATEGORIES_GET: "js:categories:get",

    /**
     * This event is emitted by the business logic once it loaded all available categories.
     * @constant
     */
    CATEGORIES_GET_LOADED: "js:categories:get:loaded",

    /**
     * This event is emitted by the native part and tells the business logic to obtain all the preferred categories.
     * @constant
     */
    CATEGORIES_GET_PREFERRED: "js:categories:get:preferred",

    /**
     * This event is emitted by the business logic and tells the native part which are the preferred categories for the
     * current user.
     * @constant
     */
    CATEGORIES_GET_PREFERRED_LOADED: "js:categories:get:preferred:loaded",

    /**
     * This event tells the native part that openPreferences page is ready to be opened.
     * @constant
     */
    MENUITEMS_OPENPREFERENCES: "js:menuitems:openPreferences",

    /**
     * This event tells the native part that openGenius page is ready to be opened.
     * @constant
     */
    MENUITEMS_OPENGENIUS: "js:menuitems:openGenius",

    /**
     * This event is emitted by the native part and tells the business logic to fetch news from a specified category.
     * @constant
     */
    NEWS_FETCH_FROM_CATEGORY: "js:news:get:from_category",

    /**
     * This event tells the native part which are the news from a requested category.
     * @constant
     */
    NEWS_FETCH_FROM_CATEGORY_LOADED: "js:news:get:from_category:loaded",

    /**
     * This event is emitted by the native part and tells the business logic to fetch news from preferred categories.
     * @constant
     */
    NEWS_FETCH_FROM_PREFERRED_CATEGORIES: "js:news:get:from_preferred_categories",

    /**
     * This event is emitted by the business logic and tells the native part which are the available news from
     * preferred categories.
     * @constant
     */
    NEWS_FETCH_FROM_PREFERRED_CATEGORIES_LOADED: "js:news:get:from_preferred_categories:loaded",

    /**
     * This event is emitted by the native part in order to tell the business logic to fetch the news by url.
     * @constant
     */
    NEWS_FETCH_BY_URL: "js:news:get:byurl",

    /**
     * This event is emitted by the business logic in order to notify the native part about news loaded correctly.
     * It contains the voicemynews news model.
     * @constant
     */
    NEWS_FETCH_BY_URL_LOADED: "js:news:get:byurl:loaded",

    /**
     * This event is emitted by the native app and requests the business logic to start reading the given news model.
     * @constant
     */
    NEWS_VOICE_READ: "js:news:voice:read",

    /**
     * This event is emitted by the native app and requests the business logic to read a given playlist of news.
     * @constant
     */
    NEWS_VOICE_READ_PLAYLIST: "js:news:voice:read:playlist",

    /**
     * This event is emitted by native part in order to tell
     */
    NEWS_VOICE_READ_PLAYLIST_PAUSE: "js:news:voice:read:playlist:pause",

    NEWS_VOICE_READ_PLAYLIST_PAUSED: "js:news:voice:read:playlist:paused"
});