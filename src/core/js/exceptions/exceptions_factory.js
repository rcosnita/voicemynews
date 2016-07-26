/**
 * This module provides a factory for building exception objects. Internally, it has all possible error codes listed.
 * @module voicemynews/js/exceptions/exceptions_factory
 */
"use strict";

/**
 * This class provides a generic data object which can describe error situations from the business logic.
 */
class ErrorDescriptor {
    constructor(errorCode, errorMessage) {
        this.errorCode = errorCode;
        this.description = errorMessage;
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
     * @returns {Object} an object containing errorCode and erroMessage properties. It might optionally include other
     * properties.
     */
    buildErrorDescriptor: (errorCode, errorMessage) => {
        const errDescriptor = new ErrorDescriptor(errorCode, errorMessage);
        return errDescriptor;
    },

    /**
     * Holds the identifier which notify native part that a descriptor not found error case occured
     * while trying to load categories.
     * @constant
     */
    CATEGORIES_ERR_DESC_NOTFOUND: "categories.descriptor_not_found",
}