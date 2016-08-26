/**
 * This module provides the cnn data source implementation. It is capable of parsing news from CNN RSS channels.
 * @module voicemynews/js/news/datasources/cnn_news_datasource
 */
"use strict";

const newsDataSource = require("js/news/news_datasource");
const htmlparser = require("js/third_party/htmlparser/lib/htmlparser");
const htmlhelper = require("js/news/datasources/html_helper");

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

        let htmlTag = htmlhelper.getDomTag(dom, htmlhelper.kHtmlTag);
        let bodyTag = htmlhelper.getDomTag(htmlTag.children, htmlhelper.kBodyTag);
        let containerTag = htmlhelper.getDomTag(bodyTag.children, htmlhelper.kDivTag, {"class": "pg-right-rail-tall pg-wrapper"});
        let article = htmlhelper.getDomTag(containerTag.children, "article", kArticleFilter);
        
        let urlMetaTag = htmlhelper.getDomTag(article.children, htmlhelper.kMetaTag, kArticleUrlFilter);
        this._parsedArticle.url = urlMetaTag.attribs.content;

        containerTag = htmlhelper.getDomTag(article.children, htmlhelper.kDivTag, kGenericContainerFilter);
        let headlineTag = htmlhelper.getDomTag(containerTag.children, "h1", kArticleHeadlineFilter);
        this._parsedArticle.headline = headlineTag.children[0].data;

        containerTag = htmlhelper.getDomTag(containerTag.children, htmlhelper.kDivTag, {"class": "pg-rail-tall__wrapper"});
        containerTag = htmlhelper.getDomTag(containerTag.children, htmlhelper.kDivTag, {"class": "pg-side-of-rail pg-rail-tall__side"});
        containerTag = htmlhelper.getDomTag(containerTag.children, htmlhelper.kDivTag, kArticleBodyFilter);
        containerTag = htmlhelper.getDomTag(containerTag.children, htmlhelper.kSectionTag, kArticleBodySectionFilter);
        containerTag = htmlhelper.getDomTag(containerTag.children, htmlhelper.kDivTag, kGenericContainerFilter);

        let paragraphTag = htmlhelper.getDomTag(containerTag.children, htmlhelper.kDivTag, {"class": "el__leafmedia el__leafmedia--sourced-paragraph"});
        paragraphTag = htmlhelper.getDomTag(paragraphTag.children, htmlhelper.kPTag, kArticleParagraphFilter);
        htmlhelper.getDomText(paragraphTag).forEach((p) => {
            this._parsedArticle.paragraphs.push(new newsDataSource.NewsParagraphModel(p));
        });

        let paragraphTags = htmlhelper.getDomTag(containerTag.children, htmlhelper.kDivTag, kArticleParagraphFilter, true);
        paragraphTags.forEach((pTag) => {
            htmlhelper.getDomText(pTag).forEach((p) => {
                this._parsedArticle.paragraphs.push(new newsDataSource.NewsParagraphModel(p));
            });
        });

        let containerReadAllTag = htmlhelper.getDomTag(containerTag.children, htmlhelper.kDivTag, kArticleReadAllFilter);
        paragraphTags = htmlhelper.getDomTag(containerReadAllTag.children, htmlhelper.kDivTag, kArticleParagraphFilter, true);
        paragraphTags.forEach((pTag) => {
            let heading = htmlhelper.getDomText(htmlhelper.getDomTag(pTag.children, "h3"));
            if (heading.length > 0) {
                this._parsedArticle.paragraphs.push(new newsDataSource.NewsParagraphModel(heading[0], [], 3));
                return;
            }

            htmlhelper.getDomText(pTag).forEach((p) => {
                this._parsedArticle.paragraphs.push(new newsDataSource.NewsParagraphModel(p));
            });
        });

        let footerTag = htmlhelper.getDomTag(containerTag.children, htmlhelper.kPTag, kArticleFooterFilter);
        this._parsedArticle.contributedBy = htmlhelper.getDomText(footerTag);
    }
}

module.exports = {
    CnnNewsDataSource: CnnNewsDataSource
}