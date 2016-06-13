"use strict";

/**
 * This module provides a set of custom exceptions which can be used to notify users / consumers about not implemented
 * features.
 * @module voicemynews/js/exceptions/notimplemented
 */

/**
 * This class provides a custom error which can be used to notify users about not implemented methods.
 *
 * @class NotImplementedMethodException
 * @alias module:voicemynews/js/exceptions/notimplemented.NotImplementedMethodException
 * @example
 * const NotImplemented = require("js/exceptions/notimplemented");
 * throw new NotImplemented.NotImplementedMethodException
 */
class NotImplementedMethodException extends Error {
    constructor(message) {
        message = message || NotImplementedMethodException.kDefaultMessage;
        super(message);
    }

    static get kDefaultMessage() {
        return "Not implemented ...";
    }
}

module.exports = {
    NotImplementedMethodException: NotImplementedMethodException
}