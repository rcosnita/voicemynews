/**
 * This module provides a factory for building exception objects. Internally, it has all possible error codes listed.
 * @module voicemynews/js/exceptions/exceptions_factory
 */
"use strict";

const kNoStackTraceMsg = "No stack trace available";

/**
 * This class provides a generic data object which can describe error situations from the business logic.
 */
class ErrorDescriptor {
    constructor(errorCode, errorMessage, stack) {
        this.errorCode = errorCode;
        this.description = errorMessage;
        this.stack = stack || kNoStackTraceMsg;
        this.isError = true;
    }

    toString() {
        return JSON.stringify(this);
    }
}

module.exports = {
    /**
     * This method builds a json object which accurately describes the error encountered.
     * @public
     * @method
     * @param {String} errorCode The error code which uniquely identifies the error case.
     * @param {String} errorMessage The error message which helps developers debug the error case.
     * @param {String} stack an optional stack trace if available.
     * @returns {Object} an object containing errorCode and erroMessage properties. It might optionally include other
     * properties.
     */
    buildErrorDescriptor: (errorCode, errorMessage, stack) => {
        const errDescriptor = new ErrorDescriptor(errorCode, errorMessage, stack);
        return errDescriptor;
    },

    /**
     * Holds the identifier which notify native part that a descriptor not found error case occured
     * while trying to load categories.
     * @constant
     */
    CATEGORIES_ERR_DESC_NOTFOUND: "categories.descriptor_not_found",

    /**
     * @constant
     * Holds the exception identifier which notifies native part that a http operation on a given news url
     * returned 404 status code.
     */
    NEWS_ERR_URL_NOTFOUND: "news.http.url_not_found",

    /**
     * @constant
     * Holds the exception identifier which notifies native part that a parsing exception occured.
     */
    NEWS_ERR_PARSE_INVALIDARTICLE: "news.parse.invalidarticle"
}