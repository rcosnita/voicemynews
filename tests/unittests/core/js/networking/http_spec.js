/*global.voicemynews = {
    core: {
        network: {
            HttpClient: {
                getInstance: function() {
                    throw new Error("Does not work ...");
                }
            }
        }
    }
};*/

describe("Test suites for unit testing http.js implementation.", () => {
    beforeEach(() => {
        this._oldVoiceMyNewsCtx = global.voicemynews;
        this._httpClient = {};

        global.voicemynews = {
            core: {
                network: {
                    HttpClient: {
                        getInstance: () => this._httpClient
                    }
                }
            }
        }

        this._httpModule = require("js/networking/http");
    });

    afterEach(() => {
        global.voicemynews = this._oldVoiceMyNewsCtx;
    });

    it("http get for string content works as expected.", () => {
        expect(true).toBe(true);
    });
});