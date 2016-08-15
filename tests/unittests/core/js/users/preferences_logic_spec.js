"use strict";

const EventEmitter = require("events").EventEmitter;
const EventNames = require("js/events/event_names");
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

    it(EventNames.CATEGORIES_GET_PREFERRED + " event works as expected.", (done) => {
        const expectedCategories = [{
            "name": "category 1",
        }, {
            "name": "category 2"
        }];
        const expectedCategoriesStr = JSON.stringify(expectedCategories);
        const preferredCategoriesLoader = Q.defer();

        this._userPreferences.getPreferredCategories = jasmine.createSpy();
        this._userPreferences.getPreferredCategories.and.returnValue(preferredCategoriesLoader.promise);
        this._buildEventData.and.returnValue(expectedCategoriesStr);

        this._userPreferences.init();

        preferredCategoriesLoader.resolve(expectedCategories);

        this._eventLoop.on(EventNames.CATEGORIES_GET_PREFERRED_LOADED, (preferredCategoriesStr) => {
            expect(this._userPreferences.getPreferredCategories).toHaveBeenCalledWith();
            expect(this._buildEventData).toHaveBeenCalledWith(expectedCategoriesStr);
            expect(preferredCategoriesStr).toBe(expectedCategoriesStr);
            done();
        });
        this._eventLoop.emit(EventNames.CATEGORIES_GET_PREFERRED, "");
    });

    it(EventNames.CATEGORIES_GET_PREFERRED + " event propagates exceptions.", (done) => {
        const errDesc = {
            errorCode: "Unknown", 
            description: "I don't really care right now ..."
        };
        const errDescStr = JSON.stringify(errDesc);
        const allCategoriesLoader = Q.defer();

        this._userPreferences.getPreferredCategories = jasmine.createSpy();
        this._userPreferences.getPreferredCategories.and.returnValue(allCategoriesLoader.promise);
        this._buildEventData.and.returnValue(errDescStr);

        allCategoriesLoader.reject(errDesc);

        this._userPreferences.init();

        this._eventLoop.on(EventNames.CATEGORIES_GET_PREFERRED_LOADED, (evtData) => {
            expect(evtData).toBe(errDescStr);
            expect(this._buildEventData).toHaveBeenCalledWith(errDescStr);

            done();
        });

        this._eventLoop.emit(EventNames.CATEGORIES_GET_PREFERRED, "");
    });

    /**
     * Provides a template for guaranteeing correct functionality for getPreferredCategories method.
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