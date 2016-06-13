"use strict";

const Q = require("js/third_party/q/q");
const HttpNativeClient = voicemynews.core.network.HttpClient.getInstance();

/**
 * This module provides a simple implementation for http operations which we are going to use in the project.
 * It provides a general method which is able to support all verbs and then provides some simplified wrappers
 * over it.
 * @module voicemynews/js/networking/http
 */

/**
 * This function provides a generic implementation for http request which supports all verbs and actions.
 *
 * @alias module:voicemynews/js/networking/http.request
 * @param {Object} requestDesc the request descriptor which defines what http action must be taken.
 * @param {String} requestDesc.url the request url.
 * @param {String} requestDesc.type the request http type we want to use (e.g GET, POST, PUT, DELETE, ...).
 * @param {Object} requestDesc.headers the request headers we want to use for this request.
 * @param {Object} requestDesc.content the request content we want to pass to this request. If the chosen method does not support body, content will simply be ignored.
 * @returns {Promise} a promise which can be used for handling the response of the request.
 */
let request = (requestDesc) => {
    let url = requestDesc.url;
    const queryParams = requestDesc.queryParams;

    if(!url || url.trim().length == 0) {
        throw new Error("No url provided.");
    }

    url += _encodeQueryParams(queryParams);

    const result = Q.defer()
    const headers = requestDesc.headers || {}
    const response = HttpNativeClient.get(url, headers);

    response.done((responseData) => {
        HttpNativeClient.parseResponseWithStringContent(responseData).done((responseParsed) => {
            result.resolve(responseParsed);
        });
    });

    return result.promise;
};

/**
 * This function provides an http get implementation. Internally, it wraps {@see module:voicemynews/js/networking/http.request} method.
 *
 * @alias module:voicemynews/js/networking/http.get
 * @param {String} url the url we want to get.
 * @param {Object} headers the headers we want to use in the get request.
 * @param {Object} queryParams the query parameters we want to pass to the request.
 * @returns {Promise} a promise which can be used for handling the response.
 * @example
 * const http = require("js/networking/http");
 * const resp = http.get("http://www.google.ro", {"Content-Type": "text/html"}, {"q": "search this term"});
 * resp.then((respData, respDesc) => {
 *      console.log(respData);
 *      const isOk = respDesc.statusCode == 200;
 * });
 */
let get = (url, headers, queryParams) => {
    return request({
        url: url,
        headers: headers,
        queryParams: queryParams
    });
};

/**
 * This function provides an http post implementation. Internally, it wraps {@see module:voicemynews/js/networking/http.request} method.
 *
 * @alias module:voicemynews/js/networking/http.post
 * @param {String} url the url where we are going to post the specified data.
 * @param {Object} headers the headers we want to use in the current request.
 * @param {Object} queryParams the query parameters we want to append to the specified url.
 * @param {Object} data binary or text data we want to post to the specified url.
 * @returns {Promise} a promise which can be used for handling the response.
 * @example
 * const http = require("js/networking/http");
 * const data = {"name": "John Doe"};
 * let resp = http.post("http://myapi.com/api/persons", {"Authorization": "XYZ oauth2 token"}, {}, data);
 * resp.then((respData, respDesc) => {
 *      console.log(respData);
 *      const isOk = respDesc.statusCode == 201;
 * });
 */
let post = (url, headers, queryParams, data) => {

};

/**
 * This function provides an http put implementation. Internally it wraps {@see module:voicemynews/js/networking/http.request} method.
 *
 * @alias module:voicemynews/js/networking/http.put
 * @param {String} url the url where we are going to put the specified data.
 * @param {Object} headers the headers we want to use in the current request.
 * @param {Object} queryParams the query parameters we want to append to the specified url.
 * @param {Object} data binary or text data we want to put to the specified url.
 * @returns {Promise} a promise which can be used for handling the response.
 * @example
 * const http = require("js/networking/http");
 * const data = {"name": "John Doe"};
 * let resp = http.put("http://myapi.com/api/persons", {"Authorization": "XYZ oauth2 token"}, {}, data);
 * resp.then((respData, respDesc) => {
 *      console.log(respData);
 *      const isOk = respDesc.statusCode == 204;
 * });
 */
let put = (url, headers, queryParams, data) => {

};

/**
 * This function provides an http delete implementation. Internally, it wraps {@see module:voicemynews/js/networking/http.request} method.
 *
 * @alias module:voicemynews/js/networking/http.del
 * @param {String} url the url of the resource we want to delete.
 * @param {Object} headers the headers we want to use in the delete request.
 * @param {Object} queryParams the query parameters we want to pass to the request.
 * @returns {Promise} a promise which can be used for handling the response.
 * @example
 * const http = require("js/networking/http");
 * const resp = http.delete("http://www.google.ro", {"Accept": "text/html"}, {"q": "search this term"});
 * resp.then((respData, respDesc) => {
 *      console.log(respData);
 *      const isOk = respDesc.statusCode == 200;
 * });
 */
let del = (url, headers, queryParams, data) => {

};

/**
 * This function provides a mechanism for encoding the specified query parameters.
 *
 * @param {Object} queryParams the list of query parameters which must be encoded.
 * @returns {String} a string containing all query parameters encoded.
 */
let _encodeQueryParams = (queryParams) => {
    let result = [];
    for (let key in queryParams) {
        result.push(key + "=" + (queryParams[key] ? encodeURIComponent(queryParams[key]) : ""));
    }

    return "?" + result.join("&");
}

module.exports = {
    "request": request,
    "get": get,
    "post": post,
    "put": put,
    "del": del,
};