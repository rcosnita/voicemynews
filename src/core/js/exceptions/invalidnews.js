/**
 * This module provides a set of custom exceptions which can be used by concrete datasources in case an error
 * occurs.
 * @module voicemynews/js/exceptions/invalidnews
 */
"use strict";

/**
 * This class provides a custom error which can be used to notify users about an invalid article fetched by a news
 * datasource.
 * @alias module:voicemynews/js/exceptions/invalidnews.InvalidNewsException
 */
class InvalidNewsException extends Error {
    static get kDefaultMessage() {
        return "The given news can not be parsed into voicemynews domain model.";
    }

    static get kDefaultCause() {
        return "Unknown error";
    }

    get cause() {
        return this._cause;
    }

    constructor(message, cause, originalEx) {
        super(message || InvalidNewsException.kDefaultMessage);
        this._cause = cause || InvalidNewsException.kDefaultCause;

        if (originalEx) {
            this.stack = originalEx.stack;
        }
    }
}

/**
 * Provides a custom exception which can be used to notify users / code about a datasource
 * which was requested but not found in the list of registered news.
 *
 * @alias module:voicemynews/js/exceptions/invalidnews.NewsProviderNotFoundException
 */
class NewsProviderNotFoundException extends Error {
    static get kDefaultMessage() {
        return "The requested news provider is not registered.";
    }

    get newsProviderId() {
        return this._newsProviderId;
    }

    constructor(message, newsProviderId) {
        super(message || NewsProviderNotFoundException.kDefaultMessage);
        this._newsProviderId = newsProviderId;
    }
}

module.exports = {
    InvalidNewsException: InvalidNewsException,
    NewsProviderNotFoundException: NewsProviderNotFoundException
}