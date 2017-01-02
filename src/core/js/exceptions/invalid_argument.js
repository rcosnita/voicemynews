"use strict";

/**
 * This module provides a specific exception for signaling invalid arguments exceptions.
 *
 * @module voicemynews/js/exceptions/invalid_argument
 */

/**
 * The class used to signal invalid argument exceptions.
 * @alias module:voicemynews/js/exceptions/invalid_argument.InvalidArgumentException
 */
class InvalidArgumentException extends Error {
    get invalidArg() {
        return this._invalidArg;
    }

    constructor(invalidArg) {
        super(`Invalid argument provided: ${invalidArg}`);
        this._invalidArg = invalidArg;
    }
}

module.exports = {
    InvalidArgumentException: InvalidArgumentException
}