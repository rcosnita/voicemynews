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

module.exports = {
    InvalidNewsException: InvalidNewsException
}