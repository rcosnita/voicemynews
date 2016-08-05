"use strict";

const EventNames = require("js/events/event_names");
const ExceptionsFactory = require("js/exceptions/exceptions_factory");
const CategoriesLogic = require("js/news/categories_logic").CategoriesLogic;

const EventEmitter = require("events").EventEmitter;
const Q = require("js/third_party/q/q");

const kCategoriesJsonFile = "js/news/categories_data.json";

describe("Tests suite for validating correct behaviour of categories logic module.", () => {
    beforeEach(() => {
        this._eventLoop = new EventEmitter();
        this._buildEventData = jasmine.createSpy();
        this._categoriesManager = new CategoriesLogic(this._eventLoop, this._buildEventData);
        this._preferencesManager = jasmine.createSpyObj("UserPreferencesLogic", ["getPreferredCategories"]);
        this._categoriesManager.preferencesManager = this._preferencesManager;

        this._oldRequireRaw = global.requireRaw;
        this._requireRaw = global.requireRaw = jasmine.createSpy();

        this._expectedNews = [{
            "newsId": 1,
            "headline": "Amazing launch by ChromeOS",
            "body": "Simple text describing what happened. It can be stored in any known format.",
            "urls": {
                "html": "http://www.google.com/news/chromeos-launch.html",
                "rss": "http://www.google.com/news/chromeos-launch.rss"
            }
        }, {
            "newsId": 2,
            "headline": "Amazing launch by Apple",
            "body": "Simple text describing what happened. It can be stored in any known format.",
            "urls": {
                "html": "http://www.apple.com/news/apple-launch.html",
                "rss": "http://www.apple.com/news/apple-launch.rss"
            }
        }];
        this._expectedNewsStr = JSON.stringify(this._expectedNews);
    });

    afterEach(() => {
        global.requireRaw = this._oldRequireRaw;
    });

    it("load categories ok.", (done) => {
        const categoriesObj = [{
            "categoryId": 1,
            "name": "Category 1"
        }, {
            "categoryId": 2,
            "name": "Category 2"
        }];

        this._requireRaw.and.returnValue(JSON.stringify(categoriesObj));

        const response = this._categoriesManager.getCategories();

        response.then((categoriesData) => {
            expect(this._requireRaw).toHaveBeenCalledWith(kCategoriesJsonFile);
            expect(categoriesData).not.toBe(undefined);
            expect(categoriesData.length).toBe(categoriesObj.length);
            expect(categoriesData[0].categoryId).toBe(categoriesObj[0].categoryId);
            expect(categoriesData[0].name).toBe(categoriesObj[0].name);
            expect(categoriesData[1].categoryId).toBe(categoriesObj[1].categoryId);
            expect(categoriesData[1].name).toBe(categoriesObj[1].name);

            done();
        });
    });

    it("get categories unhandled exception.", (done) => {
        const ex = new Error("Exception custom error ... should be included in reject ...");
        this._requireRaw.and.throwError(ex);

        let response = this._categoriesManager.getCategories();
        response.then(undefined, (rejectedData) => {
            expect(rejectedData).not.toBe(undefined);
            expect(rejectedData.errorCode).toBe(ExceptionsFactory.CATEGORIES_ERR_DESC_NOTFOUND);
            expect(rejectedData.description).toBe(ex.toString());

            done();
        });
    });

    it("async get categories event handled correctly.", (done) => {
        const categoriesObj = [{
            "categoryId": 1,
            "name": "Category 1"
        }, {
            "categoryId": 2,
            "name": "Category 2"
        }];

        const categoriesObjStr = JSON.stringify(categoriesObj);

        this._categoriesManager.init();
        this._requireRaw.and.returnValue(categoriesObjStr);
        this._buildEventData.and.returnValue(categoriesObjStr);

        this._eventLoop.on(EventNames.CATEGORIES_GET_LOADED, (categoriesDataStr) => {
            expect(this._requireRaw).toHaveBeenCalledWith(kCategoriesJsonFile);
            expect(categoriesDataStr).not.toBe(undefined);
            expect(this._buildEventData).toHaveBeenCalledWith(categoriesDataStr);

            const categoriesData = JSON.parse(categoriesDataStr);

            expect(categoriesData).not.toBe(undefined);
            expect(categoriesData.length).toBe(categoriesObj.length);
            expect(categoriesData[0].categoryId).toBe(categoriesObj[0].categoryId);
            expect(categoriesData[0].name).toBe(categoriesObj[0].name);
            expect(categoriesData[1].categoryId).toBe(categoriesObj[1].categoryId);
            expect(categoriesData[1].name).toBe(categoriesObj[1].name);

            done();
        });

        this._eventLoop.emit(EventNames.CATEGORIES_GET, "");
    });

    it("async get categories error encountered handled correctly.", (done) => {
        const ex = new Error("Unexpected and undesired exception ...");
        this._categoriesManager.init();
        this._requireRaw.and.throwError(ex);
        this._buildEventData.and.callFake((errDataStr) => errDataStr);

        this._eventLoop.on(EventNames.CATEGORIES_GET_LOADED, (errDataStr) => {
            expect(errDataStr).not.toBe(undefined);

            const errData = JSON.parse(errDataStr);

            expect(errData.isError).toBeTruthy();
            expect(errData.errorCode).toBe(ExceptionsFactory.CATEGORIES_ERR_DESC_NOTFOUND);
            expect(errData.description).toBe(ex.toString());

            done();
        });

        this._eventLoop.emit(EventNames.CATEGORIES_GET, "");
    });

    it("fetchNewsFromCategory works as expected for known category.", (done) => {
        const categoryId = 9;
        const newsFile = "js/news/data/news_categ_" + categoryId + ".json";

        this._requireRaw.and.returnValue(this._expectedNewsStr);
        let newsLoader = this._categoriesManager.fetchNewsFromCategory(categoryId);

        newsLoader.then((newsData) => {
            expect(this._requireRaw).toHaveBeenCalledWith(newsFile);
            expect(newsData).not.toBe(undefined);
            expect(JSON.stringify(newsData)).toBe(this._expectedNewsStr);

            done();
        });
    });

    it("fetchNewsFromCategory datasource not found.", (done) => {
        const categoryId = -1;
        const newsFile = "js/news/data/news_categ_" + categoryId + ".json";
        const ex = new Error("Unexpected exception while locating category data source...");

        this._requireRaw.and.throwError(ex);
        let newsLoader = this._categoriesManager.fetchNewsFromCategory(categoryId);

        newsLoader.then(undefined, (rejectedData) => {
            expect(this._requireRaw).toHaveBeenCalledWith(newsFile);
            expect(rejectedData).not.toBe(undefined);
            expect(rejectedData).not.toBe(undefined);
            expect(rejectedData.errorCode).toBe(ExceptionsFactory.CATEGORIES_ERR_DATASOURCE_NOTFOUND);
            expect(rejectedData.description).toBe(ex.toString());

            done();
        });
    });

    it("fetchNewsFromPreferredCategories works ok.", (done) => {
        const preferredCategoriesLoader = Q.defer();
        const expectedCategories = [{
            "categoryId": 1,
            "name": "Category 1"
        }, {
            "categoryId": 2,
            "name": "Category 2"
        }];
        
        this._preferencesManager.getPreferredCategories.and.returnValue(preferredCategoriesLoader.promise);
        this._categoriesManager.fetchNewsFromCategory = (categoryId) => {
            const newsLoader = Q.defer();

            if (categoryId == 1) {
                newsLoader.resolve(this._expectedNews[0]);
            } else if (categoryId == 2) {
                newsLoader.resolve(this._expectedNews[1]);
            }

            return newsLoader.promise;
        };

        const newsLoader = this._categoriesManager.fetchNewsFromPreferredCategories();

        preferredCategoriesLoader.resolve(expectedCategories);
        newsLoader.then((newsData) => {
            expect(this._preferencesManager.getPreferredCategories).toHaveBeenCalledWith();
            expect(newsData).not.toBe(undefined);
            expect(JSON.stringify(newsData)).toBe(this._expectedNewsStr);

            done();
        });
    });

    it("fetchNewsFromPreferredCategories fails if getPreferredCategories is rejected.", (done) => {
        const errDesc = {};
        const categoriesLoader = Q.defer();
        this._preferencesManager.getPreferredCategories.and.returnValue(categoriesLoader.promise);

        const newsLoader = this._categoriesManager.fetchNewsFromPreferredCategories();
        categoriesLoader.reject(errDesc);

        newsLoader.then(undefined, (rejectedData) => {
            expect(this._preferencesManager.getPreferredCategories).toHaveBeenCalledWith();
            expect(rejectedData).toBe(errDesc);

            done();
        });
    });

    it("fetchNewsFromPreferredCategories fails if fetchNewsFromCategory fails.", (done) => {
        const preferredCategoriesLoader = Q.defer();
        const errDesc1 = {};
        const errDesc2 = {};
        const expectedCategories = [{
            "categoryId": 1,
            "name": "Category 1"
        }, {
            "categoryId": 2,
            "name": "Category 2"
        }];
        
        this._preferencesManager.getPreferredCategories.and.returnValue(preferredCategoriesLoader.promise);
        this._categoriesManager.fetchNewsFromCategory = (categoryId) => {
            const newsLoader = Q.defer();

            if (categoryId == 1) {
                newsLoader.reject(errDesc1);
            } else if (categoryId == 2) {
                newsLoader.reject(errDesc2);
            }

            return newsLoader.promise;
        };

        const newsLoader = this._categoriesManager.fetchNewsFromPreferredCategories();

        preferredCategoriesLoader.resolve(expectedCategories);
        newsLoader.then(undefined, (rejectedData) => {
            expect(this._preferencesManager.getPreferredCategories).toHaveBeenCalledWith();
            expect(rejectedData).toBe(errDesc1);

            done();
        });
    });
});