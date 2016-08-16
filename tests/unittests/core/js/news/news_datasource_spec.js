"use strict";

const newsDataSource = require("js/news/news_datasource");
const NewsDataSourceAbstract = newsDataSource.NewsDataSourceAbstract;
const NewsImageModel = newsDataSource.NewsImageModel;
const NewsModel = newsDataSource.NewsModel;

describe("Tests suite for guaranteeing data source contract correct implementation.", () => {
    beforeEach(() => {
        this._newsSource = new NewsDataSourceAbstract();
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

    it("NewsModel correctly constructed.", () => {
        const newsId = "uniquely generated value";
        const headline = "Sample headline";
        const url = "http://mynews.com/sample/article.html";
        const images = [
            new NewsImageModel(),
            new NewsImageModel()
        ];

        let model1 = new NewsModel(newsId, headline, url, images);
        expect(model1.newsId).toBe(newsId);
        expect(model1.headline).toBe(headline);
        expect(model1.url).toBe(url);
        expect(model1.images).toBe(images);
    });

    it("NewsModel correctly constructed without images.", () => {
        const newsId = "uniquely generated value";
        const headline = "Sample headline";
        const url = "http://mynews.com/sample/article.html";

        let model1 = new NewsModel(newsId, headline, url);
        expect(model1.newsId).toBe(newsId);
        expect(model1.headline).toBe(headline);
        expect(model1.url).toBe(url);
    });

    it("fetchNews not implemented.", () => {
        try {
            this._newsSource.fetchNews("sample link.", {});
            expect(true).toBeFalsy();
        } catch(err) {
            expect(true).toBeTruthy();
        }
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