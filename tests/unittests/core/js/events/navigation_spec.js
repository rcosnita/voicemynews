"use strict";
const NavigationManager = require("js/events/navigation").NavigationManager;

describe("Tests suite for testing correct implementation of navigation manager layer.", () => {
    beforeEach(() => {
        this._navigationBinding = jasmine.createSpyObj("NavigationBinding", ["navigateByEvent"]);
        this._buildEventDataArgs = [];
        this._buildEventData = (evtData) => {
            this._buildEventDataArgs.push(evtData);
            return JSON.parse(evtData);
        };

        this._navigationManager = new NavigationManager(this._navigationBinding, 
            (evtData) => this._buildEventData(evtData));
    });

    it("Test navigateTo no exception raised.", () => {
        const evtData = {"prop1": "a", "prop2": "b"};
        const evtName = "custom:event";
        this._navigationManager.navigateTo(evtName, evtData);

        expect(this._buildEventDataArgs.length).toBe(1);
        expect(this._buildEventDataArgs[0]).toBe(JSON.stringify(evtData));
        expect(this._navigationBinding.navigateByEvent).toHaveBeenCalledWith(evtName, evtData);
    });

    it("Test navigateTo when navigateByEvent raises an exception.", () => {
        const ex = new Error("Unexpected navigation exception.");
        const evtData = {"prop1": 1};
        const evtName = "custom:event:2";
        
        let caughtErr = undefined;

        this._navigationBinding.navigateByEvent.and.throwError(ex);

        try {
            this._navigationManager.navigateTo(evtName, evtData);
        } catch(err) {
            caughtErr = err;
        }

        expect(caughtErr).not.toBe(undefined);
        expect(caughtErr).toBe(ex);
        expect(this._buildEventDataArgs.length).toBe(1);
        expect(this._buildEventDataArgs[0]).toBe(JSON.stringify(evtData));
        expect(this._navigationBinding.navigateByEvent).toHaveBeenCalledWith(evtName, evtData);
    });
});