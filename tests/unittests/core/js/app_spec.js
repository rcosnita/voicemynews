"use strict";

const EventEmitter = require("events");
const EventNames = require("js/events/event_names");

class EventLoopMock {
    constructor() {
        this._pendingEvents = [];
        this._listeners = {};
        this._processedEvents = [];
    }

    get processedEvents() {
        return this._processedEvents;
    }

    on(evtName, listener) {
        if(!this._listeners[evtName]) {
            this._listeners[evtName] = [];
        }

        this._listeners[evtName].push(listener);
    }

    emit(evtName, evtData) {
        this._pendingEvents.push([evtName, evtData]);
    }

    processEvents() {
        for(var idx in this._pendingEvents) {
            let evtDesc = this._pendingEvents[idx];

            for (var listenerIdx in this._listeners[evtDesc[0]]) {
                this._listeners[evtDesc[0]][listenerIdx](evtDesc[1]);

                this._processedEvents.push(evtDesc[0]);
            }
        }

        this._pendingEvents = [];
    }

    clear() {
        this._pendingEvents = [];
        this._listeners = {};
    }
}

describe("Test suites for unit testing app.js implementation.", () => {
    beforeAll(() => {
        this._oldVoiceMyNewsCtx = global.voicemynews;
        this._eventLoop = new EventLoopMock();
        this._eventLoopPlatform = jasmine.createSpyObj("EventLoopPlatform", ["getInstance", "buildEvent"]);
        this._navigationManagerPlatform = jasmine.createSpyObj("NavigationBinding", ["getInstance"]);
        this._eventLoopPlatform.getInstance.and.returnValue(this._eventLoop);

        global.voicemynews = {
            core: {
                events: {
                    EventLoopPlatform: this._eventLoopPlatform,
                    NavigationManagerPlatform: this._navigationManagerPlatform
                }
            }
        }
    });

    beforeEach(() => {
        this._eventLoop.clear();
    });

    afterAll(() => {
        global.voicemynews = this._oldVoiceMyNewsCtx;
    });

    /**
     * Internally, we inverted the order in which we emit start / shutdown so that we don't end up having an infinite
     * loop. It is acceptable in the test, because we are single threaded. In the app we have two threads and this is
     * why we don't end up with an infinite loop.
     */
    it("This test case ensures start / shutdown events are correctly handled by the app.", () => {
        this._startData = undefined;
        this._eventLoop.on(EventNames.APP_START, (evtData) => {
            this._startData = evtData;
        });

        this._eventLoop.emit(EventNames.APP_SHUTDOWN, {});

        require("js/app");

        expect(this._startData).not.toBe({});
        expect(this._eventLoop.processedEvents.length).toBe(2);
        expect(this._eventLoop.processedEvents[0]).toBe(EventNames.APP_SHUTDOWN)
        expect(this._eventLoop.processedEvents[1]).toBe(EventNames.APP_START);
    });
});