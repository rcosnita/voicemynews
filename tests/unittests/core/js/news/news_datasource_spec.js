"use strict";

const newsDataSource = require("js/news/news_datasource");
const NewsDataSourceAbstract = newsDataSource.NewsDataSourceAbstract;
const NewsImageModel = newsDataSource.NewsImageModel;
const NewsParagraphModel = newsDataSource.NewsParagraphModel;
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
        expect(model1.subheadingLevel).toBe(undefined);
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