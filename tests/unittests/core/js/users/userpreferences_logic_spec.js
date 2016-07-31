"use strict";

const EventEmitter = require("events").EventEmitter;
const ExceptionsFactory = require("js/exceptions/exceptions_factory");
const Preferences = require("js/users/preferences_logic");
const Q = require("js/third_party/q/q");

describe("tests suite for checking the functionality of user preferences.", () => {
    beforeEach(() => {
        this._eventLoop = new EventEmitter();
        this._buildEventData = jasmine.createSpy();
        this._categoriesLogic = jasmine.createSpyObj("CategoriesLogic", ["getCategories"]);
        this._userPreferences = new Preferences.UserPreferencesLogic(this._eventLoop, this._buildEventData,
            this._categoriesLogic);
    });

    it("getPreferredCategories works as expected.", (done) => {
        const allCategories = [{
                "categoryId": 1,
                "name": "Breaking News",
                "icon": "/Assets/icons/categories/breaking-news.png"
            }, {
                "categoryId": 6,
                "name": "Newspapers",
                "icon": "/Assets/icons/categories/newspapers.png"
            }, {
                "categoryId": 9,
                "name": "Technology",
                "icon": "/Assets/icons/categories/technology.png"
            }, {
                "categoryId": 10,
                "name": "Traffic",
                "icon": "/Assets/icons/categories/traffic.png"
            }, {
                "categoryId": 11,
                "name": "Weblogs",
                "icon": "/Assets/icons/categories/weblogs.png"
            }];

        const expectedCategories = [allCategories[0], allCategories[2], allCategories[4]];
        _checkGetPreferredCategoriesTemplate(allCategories, expectedCategories, done);
    });

    it("getPreferredCategories undefined categories returns empty list.", (done) => {
        _checkGetPreferredCategoriesTemplate(undefined, [], done);
    });

    it("getPreferredCategories empty categories returns empty list.", (done) => {
        _checkGetPreferredCategoriesTemplate([], [], done);
    });

    it("getPreferredCategories internal exception.", (done) => {
        const expectedErr = new Error("Unexpected error ... with some additional info ...");
        this._categoriesLogic.getCategories.and.throwError(expectedErr);

        const categoriesLoader = this._userPreferences.getPreferredCategories();

        categoriesLoader.then(undefined, (errData) => {
            expect(errData).not.toBe(undefined);
            expect(errData.errorCode).toBe(ExceptionsFactory.CATEGORIES_ERR_DESC_NOTFOUND);
            expect(errData.description).toBe(expectedErr.toString());
            done();
        })
    });

    it("getPreferredCategories getCategories rejected.", (done) => {
        const allCategoriesLoader = Q.defer();
        const rejectedInfo = {};
        this._categoriesLogic.getCategories.and.returnValue(allCategoriesLoader.promise);

        const categoriesLoader = this._userPreferences.getPreferredCategories();
        allCategoriesLoader.reject(rejectedInfo);

        categoriesLoader.then(undefined, (exInfo) => {
            expect(exInfo).toBe(rejectedInfo);
            done();
        });
    });

    /**
     * Provides a template for guaranteeing correct functionality for getPrefferedCategories method.
     */
    let _checkGetPreferredCategoriesTemplate = (allCategories, expectedCategories, done) => {
        const allCategoriesLoader = Q.defer();
        this._categoriesLogic.getCategories.and.returnValue(allCategoriesLoader.promise);

        const categoriesLoader = this._userPreferences.getPreferredCategories();

        expect(categoriesLoader).not.toBe(undefined);
        allCategoriesLoader.resolve(allCategories);

        categoriesLoader.then((preferredCategories) => {
            expect(preferredCategories).not.toBe(undefined);
            expect(preferredCategories.length).toBe(expectedCategories.length);

            for (let idx = 0; idx < preferredCategories.length; idx++) {
                expect(preferredCategories[idx]).toBe(expectedCategories[idx]);
            }

            expect(this._categoriesLogic.getCategories).toHaveBeenCalledWith();
            done();
        });
    }
});