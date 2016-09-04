"use strict";

const EventNames = require("js/events/event_names");
const newsLogic = require("js/news/news_logic");
const newsModel = require("js/news/news_datasource");
const newsException = require("js/exceptions/invalidnews");

const EventEmitter = require("events").EventEmitter;
const Q = require("js/third_party/q/q");

describe("Tests suite for ensuring news_logic works as expected.", () => {
    beforeEach(() => {
        this._eventLoop = new EventEmitter();
        this._newsProviderDataSource = jasmine.createSpyObj("NewsDataSource", ["fetchNews"]);
        this._newsProviders = {};
        this._newsProviderId = "customProvider";
        this._newsProviders[this._newsProviderId] = this._newsProviderDataSource;
        this._buildEventData = jasmine.createSpy();
        this._newsManager = new newsLogic.NewsLogic(this._eventLoop, this._buildEventData, this._newsProviders);
    });

    it("fetchNewsByUrl ok.", (done) => {
        const url = "http://www.news.com/2010/01/great_news.html";
        const rssDesc = {};
        const expectedModel = new newsModel.NewsModel();
        const fetchNewsLoader = Q.defer();

        this._newsProviderDataSource.fetchNews.and.returnValue(fetchNewsLoader.promise);
        let newsModelLoader = this._newsManager.fetchNewsByUrl(url, this._newsProviderId, rssDesc);
        fetchNewsLoader.resolve(expectedModel);

        newsModelLoader.then((actualNewsModel) => {
            expect(actualNewsModel).not.toBe(undefined);
            expect(this._newsProviderDataSource.fetchNews).toHaveBeenCalledWith(url, rssDesc);
            expect(actualNewsModel).toBe(expectedModel);

            done();
        });
    });

    it("fetchNewsByUrl unknown provider.", () => {
        const newsProviderId = "unknownProvider";

        try {
            this._newsManager.fetchNewsByUrl("http://www.google.com", newsProviderId, {});
            expect(true).toBeFalsy();
        } catch(err) {
            expect(err.message).toBe(newsException.NewsProviderNotFoundException.kDefaultMessage);
            expect(err.newsProviderId).toBe(newsProviderId);
        }
    });

    it("fetchNewsByUrl unexpected error in datasource fetching operation.", (done) => {
        const url = "http://www.news.com/2010/01/great_news.html";
        const rssDesc = {};
        const rejectionResult = {};
        const fetchNewsLoader = Q.defer();

        this._newsProviderDataSource.fetchNews.and.returnValue(fetchNewsLoader.promise);
        let newsModelLoader = this._newsManager.fetchNewsByUrl(url, this._newsProviderId, rssDesc);
        fetchNewsLoader.reject(rejectionResult);

        newsModelLoader.then(undefined, (actualRejectionResult) => {
            expect(actualRejectionResult).not.toBe(undefined);
            expect(this._newsProviderDataSource.fetchNews).toHaveBeenCalledWith(url, rssDesc);
            expect(actualRejectionResult).toBe(rejectionResult);

            done();
        });
    });

    it("fetch news triggered by event ok.", (done) => {
        const evtData = {
            "newsId": "574429b4-028e-4462-8cb8-f219fbd0e14e",
            "headline": "Breaking News - Article 1.",
            "url": "http://edition.cnn.com/2016/08/16/sport/rio-2016-nikki-hamblin-abbey-dagostino-olympic-spirit/index.html",
            "provider": this._newsProviderId,
            "images": {
                "small": {
                    "url": "http://i2.cdn.turner.com/cnnnext/dam/assets/160809155818-donald-trump-nc-rally-large-11.jpg",
                    "width": 300,
                    "height": 300
                }
            }
        };
        const evtDataStr = JSON.stringify(evtData);
        const loader = Q.defer();
        const newsLoader = Q.defer();
        const actualNewsModel = new newsModel.NewsModel(evtData.url, "Sample headline", evtData.headline);
        const actualNewsModelStr = JSON.stringify(actualNewsModel);

        this._newsProviderDataSource.fetchNews.and.returnValue(newsLoader.promise);
        this._buildEventData.and.returnValue(actualNewsModelStr);

        this._newsManager.init();

        this._eventLoop.on(EventNames.NEWS_FETCH_BY_URL_LOADED, (newsModelStr) => {
            expect(newsModelStr).toBe(actualNewsModelStr);
            expect(this._newsProviderDataSource.fetchNews.calls.count()).toBe(1);
            let args = this._newsProviderDataSource.fetchNews.calls.mostRecent().args;
            expect(args.length).toBe(2);
            expect(args[0]).toBe(evtData.url);
            expect(JSON.stringify(args[1])).toBe(evtDataStr);
            expect(this._buildEventData).toHaveBeenCalledWith(actualNewsModelStr);
            done();
        });

        this._eventLoop.emit(EventNames.NEWS_FETCH_BY_URL, {"evtData": evtDataStr});

        newsLoader.resolve(actualNewsModel);
    });

    it("fetch news triggered by event failed.", (done) => {
        const evtData = {
            "newsId": "574429b4-028e-4462-8cb8-f219fbd0e14e",
            "headline": "Breaking News - Article 1.",
            "url": "http://edition.cnn.com/2016/08/16/sport/rio-2016-nikki-hamblin-abbey-dagostino-olympic-spirit/index.html",
            "provider": this._newsProviderId,
            "images": {
                "small": {
                    "url": "http://i2.cdn.turner.com/cnnnext/dam/assets/160809155818-donald-trump-nc-rally-large-11.jpg",
                    "width": 300,
                    "height": 300
                }
            }
        };
        const evtDataStr = JSON.stringify(evtData);
        const actualErrDesc = {
            "error": "Sample error",
            "customAttr": "customAttr value."
        }
        const actualErrDescStr = JSON.stringify(actualErrDesc);
        const newsLoader = Q.defer();

        this._newsProviderDataSource.fetchNews.and.returnValue(newsLoader.promise);
        this._buildEventData.and.returnValue(actualErrDescStr);

        this._newsManager.init();

        this._eventLoop.on(EventNames.NEWS_FETCH_BY_URL_LOADED, (errDescStr) => {
            expect(errDescStr).toBe(actualErrDescStr);
            expect(this._newsProviderDataSource.fetchNews.calls.count()).toBe(1);
            let args = this._newsProviderDataSource.fetchNews.calls.mostRecent().args;
            expect(args.length).toBe(2);
            expect(args[0]).toBe(evtData.url);
            expect(JSON.stringify(args[1])).toBe(evtDataStr);
            expect(this._buildEventData).toHaveBeenCalledWith(actualErrDescStr);
            done();
        });

        this._eventLoop.emit(EventNames.NEWS_FETCH_BY_URL, {"evtData": evtDataStr});

        newsLoader.reject(actualErrDesc);
    });
});