"use strict";

const analytics = require("js/analytics/analytics_logic");
const InvalidArgumentException = require("js/exceptions/invalid_argument").InvalidArgumentException;

describe("analytics/analytics_logic tests suite.", () => {
    beforeEach(() => {
        this._oldVoiceMyNewsCtx = global.voicemynews;
        this._analyticsNative = jasmine.createSpyObj("AnalyticsNative", ["buildEvent", "logEvent"]);

        global.voicemynews = {
            core: {
                analytics: {
                    AnalyticsFactory: {
                        getInstance: () => this._analyticsNative
                    }
                }
            }
        }

        this._analyticsInst = analytics.init();
        expect(this._analyticsInst).not.toBe(undefined);
    });

    it("logEvent works ok.", () => {
        const evt = {
            eventCategory: "sample category",
            eventAction: "sample action",
            eventLabel: "Event 1",
            eventValue: -20
        };

        const evtNative = {};

        this._analyticsNative.buildEvent.and.returnValue(evtNative);

        this._analyticsInst.logEvent(evt);
        expect(this._analyticsNative.buildEvent).toHaveBeenCalledWith(2, evt.eventCategory, evt.eventAction, evt.eventLabel,
            evt.eventValue);
        expect(this._analyticsNative.logEvent).toHaveBeenCalledWith(evtNative);
    });

    let logEventMissingArgTpl = (evt, missingArgName) => {
        let caughtErr;

        try {
            this._analyticsInst.logEvent(evt);
        } catch (err) {
            caughtErr = err;
        }

        expect(caughtErr).not.toBe(undefined);
        expect(caughtErr instanceof InvalidArgumentException).toBeTruthy();
        expect(caughtErr.invalidArg).toBe(missingArgName);
    }

    it("logEvent - no event provided", () => {
        logEventMissingArgTpl(undefined, "evt");
        logEventMissingArgTpl(null, "evt");
    });

    it("logEvent - no event category provided", () => {
        logEventMissingArgTpl({}, "evt.eventCategory");
    });

    it("logEvent - no event action provided", () => {
        logEventMissingArgTpl({"eventCategory": "category"}, "evt.eventAction");
    });

    it("logEvent - no event label provided", () => {
        logEventMissingArgTpl({
            "eventCategory": "category",
            "eventAction": "action"
        }, "evt.eventLabel");
    });

    it("logEvent - no event value provided", () => {
        logEventMissingArgTpl({
            "eventCategory": "category",
            "eventAction": "action",
            "eventLabel": "label"
        }, "evt.eventValue");
    });

    it("logEvent - buildEvent unexpected exception bubbled up.", () => {
        const err = new Error();
        let caughtErr;

        this._analyticsNative.buildEvent.and.callFake(() => {
            throw err;
        });

        try {
            this._analyticsInst.logEvent({
                eventCategory: "sample category",
                eventAction: "sample action",
                eventLabel: "Event 1",
                eventValue: -20
            });
        } catch (ex) {
            caughtErr = ex;
        }

        expect(caughtErr).toBe(err);
    });

    it("logEvent - logEvent unexpected exception bubbled up.", () => {
        const err = new Error();
        let caughtErr;

        this._analyticsNative.logEvent.and.callFake(() => {
            throw err;
        });

        try {
            this._analyticsInst.logEvent({
                eventCategory: "sample category",
                eventAction: "sample action",
                eventLabel: "Event 1",
                eventValue: -20
            });
        } catch (ex) {
            caughtErr = ex;
        }

        expect(caughtErr).toBe(err);
    });

    it("logEvent screen tracking works as expected.", () => {
        const evt = {screenName: "my screen"};
        const nativeEvt = {};

        this._analyticsNative.buildEvent.and.returnValue(nativeEvt);

        this._analyticsInst.logEvent(evt);

        expect(this._analyticsNative.buildEvent).toHaveBeenCalledWith(1, evt.screenName, "", "", 0);
        expect(this._analyticsNative.logEvent).toHaveBeenCalledWith(nativeEvt);
    });

    it("logEvent - buildEvent unexpected exception bubbled up for screen tracking.", () => {
        const err = new Error();
        let caughtErr;

        this._analyticsNative.buildEvent.and.callFake(() => {
            throw err;
        });

        try {
            this._analyticsInst.logEvent({
                screenName: "my screen"
            });
        } catch (ex) {
            caughtErr = ex;
        }

        expect(caughtErr).toBe(err);
    });

    it("logEvent - logEvent unexpected exception bubbled up for screen tracking.", () => {
        const err = new Error();
        let caughtErr;

        this._analyticsNative.logEvent.and.callFake(() => {
            throw err;
        });

        try {
            this._analyticsInst.logEvent({
                screenName: "my screen"
            });
        } catch (ex) {
            caughtErr = ex;
        }

        expect(caughtErr).toBe(err);
    });
});