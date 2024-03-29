"use strict";

const NotImplemented = require("js/exceptions/notimplemented");
const NotImplementedMethodException = NotImplemented.NotImplementedMethodException;
const Q = require("q/q");

describe("Test suites for unit testing http.js implementation.", () => {
    const okRequestHeaders = { "Accept": "application/json" };
    const okRequestQueryParams = {
        "p1": "ala bala",
        "p2": "test",
        "p3": undefined
    };
    const okRequestQueryParamsStr = "p1=ala%20bala&p2=test&p3=";
    const okResponse = {
        getStatusCode: () => 200,
        getReason: () => "OK",
        getContent: () => "Works just fine",
        getHeaders: () => {
            customHeader: "application/json"
        }
    };

    beforeAll(() => {
        this._httpRequired = false;
        this._httpClient = jasmine.createSpyObj("HttpClient", ["get", "parseResponseWithStringContent", "getNewHeadersMap"]);
        this._httpModule = require("js/networking/http");
        this._httpModule.__setHttpNativeClient(this._httpClient);
    });

    beforeEach(() => {
        this._httpClient.getNewHeadersMap.and.returnValue({
            "insert": function(key, value) {
                this[key] = value;
            }
        });
    });

    afterEach(() => {
        this._httpClient.get.calls.reset();
        this._httpClient.parseResponseWithStringContent.calls.reset();
    });

    it("http get for string content works as expected.", (done) => {
        _testGetMethodTemplate(okRequestHeaders, okRequestQueryParams, okRequestQueryParamsStr,
            okResponse.getStatusCode(), okResponse.getReason(), okResponse.getContent(), okResponse.getHeaders(),
            done);
    });

    it("http get no url provided.", () => {
        expect(() => this._httpModule.get()).toThrowError("No url provided.");
    });

    it("http get no headers provided.", (done) => {
        _testGetMethodTemplate(undefined, okRequestQueryParams, okRequestQueryParamsStr,
            okResponse.getStatusCode(), okResponse.getReason(), okResponse.getContent(), okResponse.getHeaders(),
            done);
    });

    it("http get empty headers provided.", (done) => {
        _testGetMethodTemplate({}, okRequestQueryParams, okRequestQueryParamsStr,
            okResponse.getStatusCode(), okResponse.getReason(), okResponse.getContent(), okResponse.getHeaders(),
            done);
    });

    it("http get no queryParams provided.", (done) => {
        _testGetMethodTemplate({}, undefined, "",
            okResponse.getStatusCode(), okResponse.getReason(), okResponse.getContent(), okResponse.getHeaders(),
            done);
    });

    it("http get empty queryParams provided.", (done) => {
        _testGetMethodTemplate({}, {}, "",
            okResponse.getStatusCode(), okResponse.getReason(), okResponse.getContent(), okResponse.getHeaders(),
            done);
    });

    it("http post verb not implemented.", () => {
        expect(() => this._httpModule.post()).toThrowError(NotImplementedMethodException, NotImplementedMethodException.kDefaultMessage);
    });

    it("http put verb not implemented.", () => {
        expect(() => this._httpModule.put()).toThrowError(NotImplementedMethodException, NotImplementedMethodException.kDefaultMessage);
    });

    it("http delete verb not implemented.", () => {
        expect(() => this._httpModule.del()).toThrowError(NotImplementedMethodException, NotImplementedMethodException.kDefaultMessage);
    });

    /**
     * This functions provides a template for testing the get method from http module.
     */
    let _testGetMethodTemplate = (requestHeaders, queryParams, expectedQueryParamsStr, expectedStatusCode,
        expectedReason, expectedContent, expectedHeaders, doneCb) => {
        let url = "http://www.google.ro";

        this._httpClient.parseResponseWithStringContent.and.callFake((responseData, callback) => {
            expect(responseData).toBe(okResponse);

            callback(responseData);
        });

        this._httpClient.get.and.callFake((urlArg, headersArg, callback) => {
            const expectedUrl = url + (expectedQueryParamsStr.length === 0 ? "" : ("?" + expectedQueryParamsStr));
            expect(urlArg).toBe(expectedUrl);
            expect(JSON.stringify(headersArg)).toBe(JSON.stringify(requestHeaders || {}));

            callback(okResponse);
        });

        this._httpModule.get(url, requestHeaders, queryParams).done((responseParsed) => {
            expect(this._httpClient.get.calls.count()).toBe(1);
            expect(this._httpClient.parseResponseWithStringContent.calls.count()).toBe(1);

            expect(responseParsed.getStatusCode()).toBe(expectedStatusCode)
            expect(responseParsed.getReason()).toBe(expectedReason);
            expect(responseParsed.getContent()).toBe(expectedContent);
            expect(responseParsed.getHeaders()).toBe(expectedHeaders);

            doneCb();
        });
    }
});