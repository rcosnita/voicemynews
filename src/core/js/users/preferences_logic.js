/**
 * This module provides the business logic for managing user preferences. It provides the model and controller
 * for all user preferences.
 * @module voicemynews/js/news/preferences_logic
 */
"use strict";

const EventNames = require("js/events/event_names");
const ExceptionsFactory = require("js/exceptions/exceptions_factory");
const Q = require("js/third_party/q/q");

const kPreferredCategoryIds = [1, 9, 11];

/**
 * This class provides the implementation for user preferences. It provides various methods which can be used
 * in the business logic or can be invoked from C++ native part through events.
 * @alias module:voicemynews/js/users/preferences_logic.UserPreferencesLogic
 */
class UserPreferencesLogic {
    constructor(eventLoop, buildEventData, categoriesLogic) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
        this._categoriesLogic = categoriesLogic;
    }

    init() {
        this._eventLoop.on(EventNames.CATEGORIES_GET_PREFERRED, (evtData) => {
            this._handleGetCategoriesPreferredEvent();
        });
    }

    /**
     * This method returns user preferred categories.
     * @public
     * @method
     */
    getPreferredCategories() {
        const loader = Q.defer();
        try {
            const allCategories = this._categoriesLogic.getCategories();

            allCategories.then((categoriesData) => {
                categoriesData = categoriesData || [];
                let preferredCategories = categoriesData.filter(
                    (category) => kPreferredCategoryIds.indexOf(category.categoryId) >= 0);
                loader.resolve(preferredCategories);
            }, (categoriesExInfo) => {
                loader.reject(categoriesExInfo);
            });
        } catch(err) {
            var errDesc = ExceptionsFactory.buildErrorDescriptor(ExceptionsFactory.CATEGORIES_ERR_DESC_NOTFOUND,
                err.toString());

            loader.reject(errDesc);
        }

        return loader.promise;
    }

    /**
     * Correctly loads the preferred categories and send them to the native part when loaded.
     * @private
     * @method
     */
    _handleGetCategoriesPreferredEvent() {
        const categoriesLoader = this.getPreferredCategories();

        categoriesLoader.then((categories) => {
            const categoriesData = JSON.stringify(categories);
            this._eventLoop.emit(EventNames.CATEGORIES_GET_PREFERRED_LOADED, this._buildEventData(categoriesData));
        }, (rejectInfo) => {
            const errDescStr = JSON.stringify(rejectInfo);
            this._eventLoop.emit(EventNames.CATEGORIES_GET_PREFERRED_LOADED, this._buildEventData(errDescStr));
        });
    }
}

module.exports = {
    UserPreferencesLogic: UserPreferencesLogic,
    init: (eventLoop, buildEventData, categoriesLogic) => {
        const preferencesLogic = new UserPreferencesLogic(eventLoop, buildEventData, categoriesLogic);
        preferencesLogic.init();
        return preferencesLogic;
    }
}