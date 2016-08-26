/**
 * This module provides a set of helper methods used by datasources which need to parse html news.
 * @module voicemynews/js/news/datasources/html_helper
 */
"use strict";

/**
 * Given a dom object obtained by htmlparser this method tries to extract the requested tag.
 *
 * @param {Object} dom a parsed representation of html text built by htmlparser.
 * @param {String} tagName the tag we want to extract from the given dom.
 * @param {Object} tagAttribsFilter a json object containing key / value filters. The key holds the attribute
 * name and the value holds the exact value we want to match.
 * @param {Boolean} manyMode a flag which tells if we are interested in the first appearance of the tag or
 * we want to collect all appearances.
 * @return {Object} an array or a single object which matched the filtering criterias. If no result matches the
 * requested criteria, undefined is returned.
 * @alias module:voicemynews/js/news/datasources/html_helper.getDomTag
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
 * Provides the algorithm for extracting all text content from the given dom fragment.
 *
 * @param {Object} dom the dom representation of a html fragment built by htmlparser.
 * @return {Array} all text nodes belonging to the given fragment or undefined if no text is found.
 * @alias module:voicemynews/js/news/datasources/html_helper.getDomText
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

/**
 * Provides the algorithm for extracting all heading elements from the given tag. It scans from h1 till h6 tags.
 *
 * @param {Object} dom the dom fragment from which we want to extract the heading elements.
 * @param {Function} callback the callback method which is invoked for every matched heading element.
 * It receives the heading content as well as the heading level.
 * @return {Boolean} true in case at least one heading was found and false otherwise.
 * @alias module:voicemynews/js/news/datasources/html_helper.getHeadingElements
 */
let getHeadingElements = (dom, callback) => {
    let headingsCount = 0;
    
    for(let idx = 1; idx <= 6; idx++) {
        let headings = getDomText(getDomTag(dom.children, "h" + idx));

        if (!headings) {
            continue;
        }

        headingsCount += headings.length;
        headings.forEach((content) => {
            callback(content, idx);
        });
    }

    return headingsCount > 0;
};

/**
 * Provides the algorithm for extracting text content from all tags matching the given tag name and the filter.
 * For every match it invokes the callback.
 *
 * @param {Object} dom
 */
let getTextContentFromTags = (dom, tag, tagFilter, manyMode, callback) => {
    let tags = getDomTag(dom, tag, tagFilter, manyMode);

    if (manyMode) {
        tags.forEach((currTag) => {
            getDomText(currTag).forEach((content) => callback(content));
        });
    } else {
        getDomText(tags).forEach((content) => callback(content));
    }

    return tags;
};

module.exports = Object.freeze({
    /**
     * @constant
     * The html tag string value.
     */
    kHtmlTag: "html",

    /**
     * @constant
     * The body tag string value.
     */
    kBodyTag: "body",

    /**
     * @constant
     * The div tag string value.
     */
    kDivTag: "div",

    /**
     * @constant
     * The meta tag string value.
     */
    kMetaTag: "meta",

    /**
     * @constant
     * The paragraphs tag string value.
     */
    kPTag: "p",

    /**
     * @constant
     * The section tag string value.
     */
    kSectionTag: "section",

    getDomTag: getDomTag,
    getDomText: getDomText,
    getHeadingElements: getHeadingElements,
    getTextContentFromTags: getTextContentFromTags
})