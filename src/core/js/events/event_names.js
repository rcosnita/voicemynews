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
    APP_NAVIGATION_MENU_LOADED: "app:navigation_menu:loaded",

    /**
     * This event is emitted by native part and tells the business logic to obtain the available categories.
     */
    CATEGORIES_GET: "js:categories:get",

    /**
     * This event is emitted by the business logic once it loaded all available categories.
     */
    CATEGORIES_GET_LOADED: "js:categories:get:loaded",

    /**
     * This event is emitted by the native part and tells the business logic to obtain all the preferred categories.
     */
    CATEGORIES_GET_PREFERRED: "js:categories:get:preferred",

    /**
     * This event is emitted by the business logic and tells the native part which are the preferred categories for the
     * current user.
     */
    CATEGORIES_GET_PREFERRED_LOADED: "js:categories:get:preferred:loaded",

    /**
     * This event tells the native part that openPreferences page is ready to be opened.
     */
    MENUITEMS_OPENPREFERENCES: "js:menuitems:openPreferences",

    /**
     * This event tells the native part that openGenius page is ready to be opened.
     */
    MENUITEMS_OPENGENIUS: "js:menuitems:openGenius",

    /**
     * This event is emitted by the native part and tells the business logic to fetch news from a specified category.
     */
    NEWS_FETCH_FROM_CATEGORY: "js:news:get:from_category",

    /**
     * This event tells the native part which are the news from a requested category.
     */
    NEWS_FETCH_FROM_CATEGORY_LOADED: "js:news:get:from_category:loaded",

    /**
     * This event is emitted by the native part and tells the business logic to fetch news from preferred categories.
     */
    NEWS_FETCH_FROM_PREFERRED_CATEGORIES: "js:news:get:from_preferred_categories",

    /**
     * This event is emitted by the business logic and tells the native part which are the available news from
     * preferred categories.
     */
    NEWS_FETCH_FROM_PREFERRED_CATEGORIES_LOADED: "js:news:get:from_preferred_categories:loaded"
};