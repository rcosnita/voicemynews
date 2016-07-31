/**
 * This module provides the business logic for managing user preferences. It provides the model and controller
 * for all user preferences.
 * @module voicemynews/js/news/preferences_logic
 */
"use strict";

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
        throw new Error("Not implemented yet ...");
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
}

module.exports = {
    UserPreferencesLogic: UserPreferencesLogic,
    init: ((eventLoop, buildEventData) => {
        (new UserPreferencesLogic(eventLoop, buildEventData)).init();
    })
}