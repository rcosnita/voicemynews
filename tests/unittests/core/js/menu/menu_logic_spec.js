"use strict";
const EventNames = require("js/events/event_names");
const MenuLogic = require("js/menu/menu_logic").MenuLogic;
const EventEmitter = require("events");

describe("This tests suite ensures menu logic business logic works as expected.", () => {
    beforeAll(() => {
        this._menuContentObj = [{
            "icon": "/Assets/icons/menuitems/preferences.png",
            "label": "Preferences",
            "type": "item",
            "data": {
                "evtName": "js:menuitems:openPreferences"
            }
        }];
        this._menuContent = JSON.stringify(this._menuContentObj);
        this._requireRawOld = global.requireRaw;

        global.requireRaw = (menuFileName) => {
            this._menuFileName = menuFileName;
            return this._menuContent;
        }
    });

    afterAll(() => {
        global.requireRaw = this._requireRawOld;
    });

    beforeEach(() => {
        this._eventLoop = new EventEmitter();
        this._buildEventDataArgs = [];
        this._buildEventData = (evtData) => {
            this._buildEventDataArgs.push(evtData);
            return evtData;
        };
        this._navigationManager = jasmine.createSpyObj("NavigationManager", ["navigateTo"]);
        this._menuLogic = new MenuLogic(this._eventLoop, (evtData) => this._buildEventData(evtData),
                                        this._navigationManager);
        this._menuLogic.init();
    });

    it("Menu loaded ok.", () => {
        var receivedMenu = undefined;
        this._eventLoop.on(EventNames.APP_NAVIGATION_MENU_LOADED, (evtData) => {
            expect(evtData).not.toBe(undefined);
            receivedMenu = JSON.parse(evtData);
        });

        this._eventLoop.emit(EventNames.APP_NAVIGATION_MENU_LOAD);

        expect(this._buildEventDataArgs.length).toBe(1);
        expect(this._buildEventDataArgs[0]).toBe(this._menuContent);
        expect(JSON.stringify(receivedMenu)).toBe(this._menuContent);
        expect(this._menuFileName).toBe("js/menu/menu_data.json");
    });

    it("Open preferences ok.", () => {
        this._eventLoop.emit(EventNames.MENUITEMS_OPENPREFERENCES);
        expect(this._navigationManager.navigateTo).toHaveBeenCalledWith(EventNames.MENUITEMS_OPENPREFERENCES);
    });

    it("Open genius news ok.", () => {
        this._eventLoop.emit(EventNames.MENUITEMS_OPENGENIUS);
        expect(this._navigationManager.navigateTo).toHaveBeenCalledWith(EventNames.MENUITEMS_OPENGENIUS);
    });
});