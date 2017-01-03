/**
 * This module provides the business logic for loading navigation menu. 
 * @module voicemynews/js/menu/menu_logic
 */
"use strict";

const EventNames = require("js/events/event_names");
const kMenuFileName = "js/menu/menu_data.json";

/**
 * This class provides the business logic for loading the menu.
 *
 * @public
 * @class
 * @alias module:voicemynews/js/menu/menu_logic.MenuLogic
 */
class MenuLogic {
    constructor(eventLoop, buildEventData, navigationManager, analyticsLogic) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
        this._navigationManager = navigationManager;
        this._analyticsLogic = analyticsLogic;
    }

    /**
     * This method initializes the menu logic. It wires the event for loading the menu
     * on demand.
     *
     * @public
     * @method
     */
    init() {
        this._eventLoop.on(EventNames.APP_NAVIGATION_MENU_LOAD, () => this._loadMenu());
        this._eventLoop.on(EventNames.MENUITEMS_OPENPREFERENCES, (evt) => this._handleOpenPreferences(evt.evtData));
        this._eventLoop.on(EventNames.MENUITEMS_OPENGENIUS, (evt) => this._handleOpenGenius(evt.evtData));
    }

    /**
     * Loads the menu items from the menu file name. It emits a loaded menu event once
     * everything is ready.
     *
     * @private
     * @method
     */
    _loadMenu() {
        const menuData = requireRaw(kMenuFileName);

        this._eventLoop.emit(EventNames.APP_NAVIGATION_MENU_LOADED, this._buildEventData(menuData));
    }

    /**
     * This method intercepts each open preferences event emitted by the native app
     * and opens preferences page.
     */
    _handleOpenPreferences(evt) {
        evt = JSON.parse(evt);
        this._analyticsLogic.logEvent({
            screenName: evt.data.screenName
        });
        this._navigationManager.navigateTo(EventNames.MENUITEMS_OPENPREFERENCES);
    }

    /**
     * This method intercepts each open genius news event emitted by the native app
     * and opens genius news page.
     */
    _handleOpenGenius(evt) {
        evt = JSON.parse(evt);
        this._analyticsLogic.logEvent({
            screenName: evt.data.screenName
        });
        this._navigationManager.navigateTo(EventNames.MENUITEMS_OPENGENIUS);
    }
}

module.exports = {
    MenuLogic: MenuLogic,
    /**
     * Method used to initialize the menu logic of the application. Internally, it instantiates MenuLogic class
     * and wires all menu related events correctly.
     */
    init: ((eventLoop, buildEventData, navigationManager, analyticsLogic) => {
        (new MenuLogic(eventLoop, buildEventData, navigationManager, analyticsLogic)).init();
    })
}