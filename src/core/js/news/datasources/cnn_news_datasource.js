/**
 * This module provides the cnn data source implementation. It is capable of parsing news from CNN RSS channels.
 * @module voicemynews/js/news/datasources/cnn_news_datasource
 */
"use strict";

const newsDataSource = require("js/news/news_datasource");
const htmlparser = require("js/third_party/htmlparser/lib/htmlparser");

/**
 * Given a dom object obtained by htmlparser this method tries to extract the requested tag.
 */
let getDomTag = (dom, tagName, tagAttribsFilter, manyMode) => {
    if (!dom) {
        return;
    }

    tagAttribsFilter = tagAttribsFilter || {};
    let tag = undefined;
    let results = [];

    for (let idx = 0; idx < dom.length; idx++) {
        let entry = dom[idx];

        if (entry.type !== "tag" || entry.name !== tagName) {
            continue;
        }

        if (Object.keys(tagAttribsFilter).length === 0) {
            if (manyMode) {
                results.push(entry);
                continue;
            }

            return entry;
        }

        for (let filterKey in tagAttribsFilter) {
            if (!entry.attribs[filterKey]) {
                continue;
            }

            if (entry.attribs[filterKey].trim() === tagAttribsFilter[filterKey]) {
                if (!manyMode) {
                    return entry;
                }

                results.push(entry);
            }
        }
    }

    if (manyMode) {
        return results;
    }
}

/**
 * Provides the algorithm for extracting all text sections from the given dom fragment.
 *
 * @return {Array}
 */
let getDomText = (dom) => {
    let results = [];

    if (!dom) {
        return results;
    }

    dom.children.forEach((item) => {
        if (!(item.type === "text")) {
            return;
        }

        results.push(item.data);
    });

    return results;
};

const kGenericContainerFilter = {"class": "l-container"};
const kArticleFilter = {"itemtype": "http://schema.org/NewsArticle"};
const kArticleUrlFilter = {"itemprop": "url"};
const kArticleHeadlineFilter = {"class": "pg-headline"};
const kArticleBodyFilter = {"itemprop": "articleBody"};
const kArticleBodySectionFilter = {"id": "body-text"};
const kArticleParagraphFilter = {"class": "zn-body__paragraph"};
const kArticleReadAllFilter = {"class": "zn-body__read-all"};
const kArticleFooterFilter = {"class": "zn-body__paragraph zn-body__footer"};

/**
 * CNN data source class which can fetch news and parse them into voicemynews domain objects.
 * @alias module:voicemynews/js/news/datasources/cnn_news_datasource.CnnNewsDataSource
 */
class CnnNewsDataSource extends newsDataSource.NewsDataSourceAbstract {
    constructor() {
        super();
        this._handler = new htmlparser.DefaultHandler((error, dom) => this._handleHtmlCallback(error, dom));
        this._parser = new htmlparser.Parser(this._handler);
        this._parsedArticle = undefined;
    }

    /**
     * CNN is currently delivering news formatted as html pages. We scrape the html and convert it into
     * voicemynews model objects.
     */
    parseContent(article, rssDesc) {
        this._parser.parseComplete(article);
        return new newsDataSource.NewsModel("", this._parsedArticle.headline, this._parsedArticle.url, [],
            this._parsedArticle.paragraphs, this._parsedArticle.contributedBy);
    }

    _handleHtmlCallback(error, dom) {
        if (error) {
            throw new Error(error);
        }

        this._parsedArticle = {
            url: undefined,
            headline: undefined,
            paragraphs: [],
            contributedBy: undefined
        }

        let htmlTag = getDomTag(dom, "html");
        let bodyTag = getDomTag(htmlTag.children, "body");
        let containerTag = getDomTag(bodyTag.children, "div", {"class": "pg-right-rail-tall pg-wrapper"});
        let article = getDomTag(containerTag.children, "article", kArticleFilter);
        
        let urlMetaTag = getDomTag(article.children, "meta", kArticleUrlFilter);
        this._parsedArticle.url = urlMetaTag.attribs.content;

        containerTag = getDomTag(article.children, "div", kGenericContainerFilter);
        let headlineTag = getDomTag(containerTag.children, "h1", kArticleHeadlineFilter);
        this._parsedArticle.headline = headlineTag.children[0].data;

        containerTag = getDomTag(containerTag.children, "div", {"class": "pg-rail-tall__wrapper"});
        containerTag = getDomTag(containerTag.children, "div", {"class": "pg-side-of-rail pg-rail-tall__side"});
        containerTag = getDomTag(containerTag.children, "div", kArticleBodyFilter);
        containerTag = getDomTag(containerTag.children, "section", kArticleBodySectionFilter);
        containerTag = getDomTag(containerTag.children, "div", kGenericContainerFilter);

        let paragraphTag = getDomTag(containerTag.children, "div", {"class": "el__leafmedia el__leafmedia--sourced-paragraph"});
        paragraphTag = getDomTag(paragraphTag.children, "p", kArticleParagraphFilter);
        getDomText(paragraphTag).forEach((p) => {
            this._parsedArticle.paragraphs.push(new newsDataSource.NewsParagraphModel(p));
        });

        let paragraphTags = getDomTag(containerTag.children, "div", kArticleParagraphFilter, true);
        paragraphTags.forEach((pTag) => {
            getDomText(pTag).forEach((p) => {
                this._parsedArticle.paragraphs.push(new newsDataSource.NewsParagraphModel(p));
            });
        });

        let containerReadAllTag = getDomTag(containerTag.children, "div", kArticleReadAllFilter);
        paragraphTags = getDomTag(containerReadAllTag.children, "div", kArticleParagraphFilter, true);
        paragraphTags.forEach((pTag) => {
            let heading = getDomText(getDomTag(pTag.children, "h3"));
            if (heading.length > 0) {
                this._parsedArticle.paragraphs.push(new newsDataSource.NewsParagraphModel(heading[0], [], 3));
                return;
            }

            getDomText(pTag).forEach((p) => {
                this._parsedArticle.paragraphs.push(new newsDataSource.NewsParagraphModel(p));
            });
        });

        let footerTag = getDomTag(containerTag.children, "p", kArticleFooterFilter);
        this._parsedArticle.contributedBy = getDomText(footerTag);
    }
}

module.exports = {
    CnnNewsDataSource: CnnNewsDataSource
}