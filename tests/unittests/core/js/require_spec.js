"use strict";

describe("Tests suite for unit testing require.js global functions.", () => {
    beforeAll(() => {
        this._oldVoiceMyNews = global.voicemynews;
        this._oldRequire = global.require;
        this._requireInstance = jasmine.createSpyObj("RequireBinding", ["loadRaw"]);

        global.voicemynews = {
            core: {
                RequireFactory: {
                    getInstance: () => {
                        return this._requireInstance;
                    }
                }
            }
        }

        require("js/require");
    });

    beforeEach(() => {
        this._requireInstance.loadRaw.calls.reset();
    });

    afterEach(() => {
        this._requireInstance.loadRaw.calls.reset();
    });

    afterAll(() => {
        global.voicemynews = this._oldVoiceMyNews;
        global.require = this._oldRequire;
    });

    it("requireRaw ok.", () => {
        const expectedResult = "Hello world ...";
        const fileName = "test_file_xx.txt";
        this._requireInstance.loadRaw.and.returnValue(expectedResult);
        const content = requireRaw(fileName);

        expect(content).toBe(expectedResult);
        expect(this._requireInstance.loadRaw.calls.count()).toBe(1);
        expect(this._requireInstance.loadRaw).toHaveBeenCalledWith(fileName);
    });

    it("requireRaw exception.", () => {
        const fileName = "sample_file.txt";
        const expectedEx = new Error("Not found ...");
        let caughtEx = undefined;

        this._requireInstance.loadRaw.and.throwError(expectedEx);

        try {
            requireRaw(fileName);
        } catch(err) {
            caughtEx = err;
        }

        expect(caughtEx).toBe(expectedEx);
        expect(this._requireInstance.loadRaw.calls.count()).toBe(1);
        expect(this._requireInstance.loadRaw).toHaveBeenCalledWith(fileName);
    });
});