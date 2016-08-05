/**
 * This module provides the business logic for managing available news categories.
 * @module voicemynews/js/news/categories_logic
 */
"use strict";

const EventNames = require("js/events/event_names");
const ExceptionsFactory = require("js/exceptions/exceptions_factory");
const Q = require("js/third_party/q/q");

const kCategoriesJsonFile = "js/news/categories_data.json";
const kNewsByCategoriesLocation = "js/news/data/news_categ_";

/**
 * This class provides the business logic for managing the new categories available.
 *
 * @public
 * @class
 * @alias module:voicemynews/js/news/categories_logic.CategoriesLogic
 * @property {module:voicemynews/js/users/preferences_logic.UserPreferencesLogic} preferencesManager the preferences manager we can use to obtain preferred categories.
 */
class CategoriesLogic {
    constructor(eventLoop, buildEventData) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
        this._preferencesManager = undefined;
    }

    get preferencesManager() {
        return this._preferencesManager;
    }

    set preferencesManager(value) {
        this._preferencesManager = value;
    }

    /**
     * This method wires all async events which must be handled by the business logic.
     *
     * @public
     * @method
     */
    init() {
        this._eventLoop.on(EventNames.CATEGORIES_GET, () => this._handleGetCategories());
    }

    /**
     * This method obtains the categories which can be used in the application.
     *
     * @public
     * @method
     */
    getCategories() {
        const responseLoader = Q.defer();

        try {
            const categories = JSON.parse(requireRaw(kCategoriesJsonFile));

            responseLoader.resolve(categories);
        } catch(err) {
            const errDesc = ExceptionsFactory.buildErrorDescriptor(ExceptionsFactory.CATEGORIES_ERR_DESC_NOTFOUND,
                                err.toString());
            responseLoader.reject(errDesc);
        }

        return responseLoader.promise;
    }

    /**
     * Fetches all available news from the specified category.
     * @param {Number} categoryId the category unique identifier.
     * @returns {Promise} a promise object which resolves with an array of news from the specified category.
     */
    fetchNewsFromCategory(categoryId) {
        const newsLoader = Q.defer();
        try {
            const categoryNewsData = requireRaw(kNewsByCategoriesLocation + categoryId + ".json");
            newsLoader.resolve(JSON.parse(categoryNewsData));
        } catch(ex) {
            const errDesc = ExceptionsFactory.buildErrorDescriptor(ExceptionsFactory.CATEGORIES_ERR_DATASOURCE_NOTFOUND, ex.toString());

            newsLoader.reject(errDesc);
        }

        return newsLoader.promise;
    }

    /**
     * Fetches all available news from preferred categories. Preferred categories are obtained from user
     * preferences logic.
     * @returns {Promise} a promise object which resolves with an array of news belonging to the preferred categories.
     */
    fetchNewsFromPreferredCategories() {
        const newsLoader = Q.defer();
        const categoriesLoader = this._preferencesManager.getPreferredCategories();
        const pendingLoaders = [];
        let errors = undefined;
        var promiseIdx = 0;

        categoriesLoader.then((categoriesData) => {
            errors = new Array(categoriesData.length);

            categoriesData.forEach((categoryDesc) => {
                const idx = promiseIdx++;
                let innerLoader = this.fetchNewsFromCategory(categoryDesc.categoryId);

                innerLoader.then(undefined, (errDesc) => {
                    errors[idx] = errDesc;
                });

                pendingLoaders.push(innerLoader);
            });

            Q.all(pendingLoaders).then((news) => {
                newsLoader.resolve(news);
            }, () => {
                newsLoader.reject(errors[0]);
            });
        }, (rejectedData) => {
            newsLoader.reject(rejectedData);
        });

        return newsLoader.promise;
    }

    /**
     * This method get categories and emits loaded events when done.
     */
    _handleGetCategories() {
        const categoriesLoader = this.getCategories();

        categoriesLoader.then((categoriesData) => {
            let categoriesDataStr = JSON.stringify(categoriesData);
            this._eventLoop.emit(EventNames.CATEGORIES_GET_LOADED, this._buildEventData(categoriesDataStr));
        }, (errorData) => {
            let errorDataStr = errorData.toString();
            this._eventLoop.emit(EventNames.CATEGORIES_GET_LOADED, this._buildEventData(errorDataStr));
        });
    }
}

module.exports = {
    CategoriesLogic: CategoriesLogic,
    init: ((eventLoop, buildEventData) => {
        (new CategoriesLogic(eventLoop, buildEventData)).init();
    })
}