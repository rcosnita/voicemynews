/**
 * This module provides a set of helper methods used by datasources which need to parse html news.
 * @module voicemynews/js/news/datasources/html_helper
 */
"use strict";

/**
 * Given the dom element and an object holding all filters this method tells if the dom element matches requested
 * filters.
 *
 * @param {JSON} domElement a json representation of the parsed dom element.
 * @param {JSON} tagAttribsFilter a json object describing all filters which must be matched.
 * @return {Boolean} true if all filters are matched and false otherwise.
 */
let _isMatchingFilters = (domElement, filters) => {
    if (!filters || filters.length === 0) {
        return true;
    }

    const attribs = domElement.attribs || {};
    let matchedFilters = 0;
    for (let filterKey in filters) {
        if (!attribs[filterKey]) {
            return false;
        }

        if (attribs[filterKey].trim() === filters[filterKey]) {
            matchedFilters++;
        }
    }

    return matchedFilters === Object.keys(filters).length;
}

/**
 * Given a dom object children obtained by htmlparser this method tries to extract the requested tag.
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

        if (_isMatchingFilters(entry, tagAttribsFilter)) {
            if (!manyMode) {
                return entry;
            }

            results.push(entry);
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

    if (!dom || !dom.children) {
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
    if (!dom || !callback || typeof callback !== "function") {
        return false;
    }

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
 * @param {Object} dom The current children of a dom element parsed with htmlparser.
 * @param {String} tag the tag name we want to extract text from.
 * @param {JSON} tagFilter the tag filters we want to use for filtering matching tags.
 * @param {Boolean} manyMode a flag telling if we want to match multiple tags or a single one.
 * @param {function} callback the callback we want to invoke for every match. The callback receives
 * a string which represents the text from each tag.
 * @returns {Object} an array of matching tags if manyMode is true or in case input parameters are invalid.
 * Otherwise it returns the specific tag.
 */
let getTextContentFromTags = (dom, tag, tagFilter, manyMode, callback) => {
    if (!dom || !callback || typeof callback !== "function") {
        return [];
    }

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