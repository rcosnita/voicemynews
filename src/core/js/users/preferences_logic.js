/**
 * This module provides the business logic for managing user preferences. It provides the model and controller
 * for all user preferences.
 * @module voicemynews/js/news/preferences_logic
 */
"use strict";

/**
 * This class provides the implementation for user preferences. It provides various methods which can be used
 * in the business logic or can be invoked from C++ native part through events.
 * @alias module:voicemynews/js/users/preferences_logic.UserPreferencesLogic
 */
class UserPreferencesLogic {
    constructor(eventLoop, buildEventData) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
    }

    init() {
        throw new Error("Not implemented yet ...");
    }

    /**
     * This method returns user preferred categories.
     * @public
     * @method
     */
    getPreferredCategories() {
        throw new Error("Not implemented yet ...");
    }
}

module.exports = {
    UserPreferencesLogic: UserPreferencesLogic,
    init: ((eventLoop, buildEventData) => {
        (new UserPreferencesLogic(eventLoop, buildEventData)).init();
    })
}