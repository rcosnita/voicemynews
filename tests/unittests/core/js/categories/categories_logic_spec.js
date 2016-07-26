"use strict";

const EventNames = require("js/events/event_names");
const ExceptionsFactory = require("js/exceptions/exceptions_factory");
const CategoriesLogic = require("js/news/categories_logic").CategoriesLogic;

const EventEmitter = require("events").EventEmitter;

const kCategoriesJsonFile = "js/news/categories_data.json";

describe("Tests suite for validating correct behaviour of categories logic module.", () => {
    beforeEach(() => {
        this._eventLoop = new EventEmitter();
        this._buildEventData = jasmine.createSpy();
        this._categoriesManager = new CategoriesLogic(this._eventLoop, this._buildEventData);

        this._oldRequireRaw = global.requireRaw;
        this._requireRaw = global.requireRaw = jasmine.createSpy();
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
});