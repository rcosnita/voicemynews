"use strict";
const eventLoop = voicemynews.core.events.EventLoopPlatform.getInstance();
const buildEvent = voicemynews.core.events.EventLoopPlatform.buildEvent;
var isRunning = true;

eventLoop.on("shutdown", (evtData) => {
    isRunning = false;
});

eventLoop.emit("start", buildEvent(JSON.stringify("{}")));

while (isRunning) {
    eventLoop.processEvents();
}