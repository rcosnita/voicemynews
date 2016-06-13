"use strict";

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
    });

    beforeEach(() => {
        this._oldVoiceMyNewsCtx = global.voicemynews;
        this._httpClient = jasmine.createSpyObj("HttpClient", ["getInstance", "get", "parseResponseWithStringContent"]);

        global.voicemynews = {
            core: {
                network: {
                    HttpClient: this._httpClient
                }
            }
        }

        this._httpClient.getInstance.and.returnValue(this._httpClient);
        this._httpModule = require("js/networking/http");

        if (!this._httpRequired) {
            expect(this._httpClient.getInstance.calls.count()).toBe(1);

            this._httpRequired = true;
        }
    });

    afterEach(() => {
        global.voicemynews = this._oldVoiceMyNewsCtx;
    });

    it("http get for string content works as expected.", (done) => {
        let url = "http://www.google.ro";

        const result = Q.defer();
        const getParser = Q.defer();

        this._httpClient.parseResponseWithStringContent.and.callFake((responseData) => {
            expect(responseData).toBe(okResponse);
            
            setTimeout(() => {
                getParser.resolve(responseData);
            });

            return getParser.promise;
        });

        this._httpClient.get.and.callFake((urlArg, headersArg) => {
            expect(urlArg).toBe(url + "?" + okRequestQueryParamsStr);
            expect(headersArg).toBe(okRequestHeaders);

            setTimeout(() => {
                result.resolve(okResponse);
            });

            return result.promise;
        });

        this._httpModule.get(url, okRequestHeaders, okRequestQueryParams).done((responseParsed) => {
            expect(this._httpClient.get.calls.count()).toBe(1);
            expect(this._httpClient.parseResponseWithStringContent.calls.count()).toBe(1);

            expect(responseParsed.getStatusCode()).toBe(okResponse.getStatusCode())
            expect(responseParsed.getReason()).toBe(okResponse.getReason());
            expect(responseParsed.getContent()).toBe(okResponse.getContent());
            expect(responseParsed.getHeaders()).toBe(okResponse.getHeaders());

            done();
        });
    });

    it("http get no url provided.", () => {
        expect(() => this._httpModule.get()).toThrowError("No url provided.");
    });

    it("http get no headers provided.", () => {

    });

    it("http get empty headers provided.", () => {

    });

    it("http get no queryParams provided.", () => {

    });

    it("http get empty queryParams provided.", () => {

    });

    it("http post verb not implemented.", () => {

    });

    it("http put verb not implemented.", () => {

    });

    it("http delete verb not implemented.", () => {

    });
});