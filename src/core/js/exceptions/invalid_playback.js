/**
 * This module provides a set of custom exceptions which are used by voice business logic.
 * @module voicemynews/js/exceptions/invalid_playback
 */
"use strict";

/**
 * Provides the exception which notifies unsupported attempt of playing mutiple playback streams
 * in parallel.
 */
class MultiplePlaybackStreamsNotSupported extends Error {
    static get kDefaultMessage() {
        return "Multiple playback streams are not supported in parallel.";
    }

    static get kDefaultCause() {
        return "Multiple play attempts."
    }

    get cause() {
        return this.kDefaultCause;
    }

    constructor(message, cause, originalEx) {
        super(message || MultiplePlaybackStreamsNotSupported.kDefaultMessage);
        this._cause = cause || MultiplePlaybackStreamsNotSupported.kDefaultCause;

        if (originalEx) {
            this.stack = originalEx.stack;
        }
    }
}

module.exports = {
    MultiplePlaybackStreamsNotSupported: MultiplePlaybackStreamsNotSupported
}
