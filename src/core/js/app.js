/**
 * This module provides the main entry point of VoiceMyNews business logic. Internally, it starts an event loop
 * which represents the bridge between native part and js part of the app.
 * This module execution will finish only when the application shutdowns so no other module must require this
 * module.
 * @module voicemynews/js/app
 */
"use strict";

const eventLoop = voicemynews.core.events.EventLoopPlatform.getInstance();
const EventNames = require("js/events/event_names");
const buildEventData = voicemynews.core.events.EventLoopPlatform.buildEvent;
var isRunning = true;

eventLoop.on(EventNames.APP_SHUTDOWN, (evtData) => {
    isRunning = false;
});

eventLoop.emit(EventNames.APP_START, buildEventData(JSON.stringify("{}")));

require("js/menu/menu_logic").init(eventLoop, buildEventData);

while (isRunning) {
    eventLoop.processEvents();
}