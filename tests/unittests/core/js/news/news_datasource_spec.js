"use strict";

const Q = require("js/third_party/q/q");

const exceptionsFactory = require("js/exceptions/exceptions_factory");
const newsDataSource = require("js/news/news_datasource");
const NewsDataSourceAbstract = newsDataSource.NewsDataSourceAbstract;
const NewsImageModel = newsDataSource.NewsImageModel;
const NewsParagraphModel = newsDataSource.NewsParagraphModel;
const NewsModel = newsDataSource.NewsModel;

describe("Tests suite for guaranteeing data source contract correct implementation.", () => {
    beforeEach(() => {
        this._httpClient = jasmine.createSpyObj("HttpClient", ["get"]);
        this._newsSource = new NewsDataSourceAbstract(this._httpClient);
    });

    it("NewsImageModel correctly constructed.", () => {
        const url = "http://www.google.com";
        const width = 300;
        const height = 300;

        let model1 = new NewsImageModel(url, width, height);
        expect(model1.url).toBe(url);
        expect(model1.width).toBe(width);
        expect(model1.height).toBe(height);
    });

    it("NewsParagraphModel correctly constructed.", () => {
        const content = "Sample content from a simple paragraph.";
        const images = [
            new NewsImageModel(),
            new NewsImageModel()
        ];
        const subheadingLevel = 3;

        let model1 = new NewsParagraphModel(content, images, subheadingLevel);
        expect(model1.content).toBe(content);
        expect(model1.images).toBe(images);
        expect(model1.subheadingLevel).toBe(subheadingLevel);
    });

    it("NewsParagraphModel correctly constructed with default values.", () => {
        const content = "Sample paragraph.";

        let model1 = new NewsParagraphModel(content);
        expect(model1.content).toBe(content);
        expect(JSON.stringify(model1.images)).toBe("[]");
        expect(model1.subheadingLevel).toBe(-1);
    });

    it("NewsModel correctly constructed.", () => {
        const newsId = "uniquely generated value";
        const headline = "Sample headline";
        const url = "http://mynews.com/sample/article.html";
        const images = {
            "small": new NewsImageModel(),
            "medium": new NewsImageModel(),
            "others": [new NewsImageModel(), new NewsImageModel()]
        };
        const paragraphs = [
            new NewsParagraphModel(),
            new NewsParagraphModel()
        ];
        const contributedBy = [
            "Author 1",
            "Extraordinary contribution by John Doe"
        ];

        let model1 = new NewsModel(newsId, headline, url, images, paragraphs, contributedBy);
        expect(model1.newsId).toBe(newsId);
        expect(model1.headline).toBe(headline);
        expect(model1.url).toBe(url);
        expect(model1.images).toBe(images);
        expect(model1.paragraphs).toBe(paragraphs);
        expect(model1.contributedBy).toBe(contributedBy);
    });

    it("NewsModel correctly constructed with default values.", () => {
        const newsId = "uniquely generated value";
        const headline = "Sample headline";
        const url = "http://mynews.com/sample/article.html";

        let model1 = new NewsModel(newsId, headline, url);
        expect(model1.newsId).toBe(newsId);
        expect(model1.headline).toBe(headline);
        expect(model1.url).toBe(url);
        expect(JSON.stringify(model1.images)).toBe("{}");
        expect(JSON.stringify(model1.paragraphs)).toBe("[]");
        expect(JSON.stringify(model1.contributedBy)).toBe("[]");
    });

    it("fetchNews article fetched correctly.", (done) => {
        const articleBody = "Here is the article body";
        const expectedNewsModel = new NewsModel();
        const url = "http://www.cnn.com/2016/08/25/middleeast/muqawama-mosul-resistance-fighters/index.html";
        const rssDesc = {};

        const httpLoader = Q.defer();
        const response = jasmine.createSpyObj("HttpResponse", ["getStatusCode", "getContent"]);
        response.getStatusCode.and.returnValue(200);
        response.getContent.and.returnValue(articleBody);

        this._httpClient.get.and.returnValue(httpLoader.promise);
        this._newsSource.parseContent = jasmine.createSpy();
        this._newsSource.parseContent.and.returnValue(expectedNewsModel);

        let newsLoader = this._newsSource.fetchNews(url, rssDesc);
        httpLoader.resolve(response);

        newsLoader.then((newsModel) => {
            expect(newsModel).not.toBe(undefined);
            expect(this._httpClient.get).toHaveBeenCalledWith(url);
            expect(response.getStatusCode).toHaveBeenCalledWith();
            expect(response.getContent).toHaveBeenCalledWith();
            expect(this._newsSource.parseContent).toHaveBeenCalledWith(articleBody, rssDesc);
            expect(newsModel).toBe(expectedNewsModel);

            done();
        });
    });

    it("fetchNews article not found.", (done) => {
        const url = "http://not/found";

        const httpLoader = Q.defer();
        const response = jasmine.createSpyObj("HttpResponse", ["getStatusCode"]);
        response.getStatusCode.and.returnValue(404);

        this._httpClient.get.and.returnValue(httpLoader.promise);
        let newsLoader = this._newsSource.fetchNews(url, undefined);
        httpLoader.resolve(response);

        newsLoader.then(undefined, (errData) => {
            expect(this._httpClient.get).toHaveBeenCalledWith(url);
            expect(response.getStatusCode).toHaveBeenCalledWith();
            expect(errData).not.toBe(undefined);
            expect(errData.errorCode).toBe(exceptionsFactory.NEWS_ERR_URL_NOTFOUND);
            expect(errData.description).not.toBe(undefined);
            done();
        });
    });

    it("fetchNews article fetches ok, parse throws exception.", (done) => {
        const url = "http://www.cnn.com/2016/08/25/middleeast/muqawama-mosul-resistance-fighters/index.html";
        const rssDesc = {};
        const articleBody = "Invalid article";

        const originalErr = new Error("Unknown error occurred.");
        const httpLoader = Q.defer();
        const response = jasmine.createSpyObj("HttpResponse", ["getStatusCode", "getContent"]);
        response.getStatusCode.and.returnValue(200);
        response.getContent.and.returnValue(articleBody);

        this._httpClient.get.and.returnValue(httpLoader.promise);
        this._newsSource.parseContent = jasmine.createSpy();
        this._newsSource.parseContent.and.throwError(originalErr);

        let newsLoader = this._newsSource.fetchNews(url, rssDesc);
        httpLoader.resolve(response);

        newsLoader.then(undefined, (errData) => {
            expect(errData).not.toBe(undefined);
            expect(this._httpClient.get).toHaveBeenCalledWith(url);
            expect(response.getStatusCode).toHaveBeenCalledWith();
            expect(response.getContent).toHaveBeenCalledWith();
            expect(this._newsSource.parseContent).toHaveBeenCalledWith(articleBody, rssDesc);
            expect(errData.errorCode).toBe(exceptionsFactory.NEWS_ERR_PARSE_INVALIDARTICLE);
            expect(errData.description).toBe(originalErr.toString());
            expect(errData.stack).toBe(originalErr.stack);

            done();
        });
    });

    it("parseContent not implemented.", () => {
        try {
            this._newsSource.parseContent("html content in here.", {});
            expect(true).toBeFalsy();
        } catch(err) {
            expect(true).toBeTruthy();
        }
    });
});