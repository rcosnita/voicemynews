/**
 * This module provides the business logic for loading navigation menu. 
 * @module voicemynews/js/menu/menu_logic
 */
"use strict";

const EventNames = require("js/events/event_names");
const kMenuFileName = "js/menu/menu_data.json"

/**
 * This class provides the business logic for loading the menu.
 *
 * @public
 * @class
 */
class MenuLogic {
    constructor(eventLoop, buildEventData) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
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
}

module.exports = {
    /**
     * Method used to initialize the menu logic of the application. Internally, it instantiates MenuLogic class
     * and wires all menu related events correctly.
     */
    init: ((eventLoop, buildEventData) => {
        (new MenuLogic(eventLoop, buildEventData)).init();
    })
}