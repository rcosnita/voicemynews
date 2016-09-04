/**
 * This module provides the main entry point of VoiceMyNews business logic. Internally, it starts an event loop
 * which represents the bridge between native part and js part of the app.
 * This module execution will finish only when the application shutdowns so no other module must require this
 * module.
 * @module voicemynews/js/app
 */
"use strict";

const buildEventData = voicemynews.core.events.EventLoopPlatform.buildEvent;
const eventLoop = voicemynews.core.events.EventLoopPlatform.getInstance();
const EventNames = require("js/events/event_names");
const httpClient = require("js/networking/http");
const NavigationManager = require("js/events/navigation").NavigationManager;
var isRunning = true;

eventLoop.on(EventNames.APP_SHUTDOWN, (evtData) => {
    isRunning = false;
});

eventLoop.emit(EventNames.APP_START, buildEventData(JSON.stringify("{}")));

const newsProviders = {
    "cnn": new (require("js/news/datasources/cnn_news_datasource").CnnNewsDataSource)(httpClient)
}

require("js/menu/menu_logic").init(eventLoop, buildEventData, new NavigationManager(undefined, buildEventData));
const categoriesLogic = require("js/news/categories_logic").init(eventLoop, buildEventData);
const preferencesLogic = require("js/users/preferences_logic").init(eventLoop, buildEventData, categoriesLogic);
categoriesLogic.preferencesManager = preferencesLogic;
const newsLogic = require("js/news/news_logic").init(eventLoop, buildEventData, newsProviders);

while (isRunning) {
    eventLoop.processEvents();
}