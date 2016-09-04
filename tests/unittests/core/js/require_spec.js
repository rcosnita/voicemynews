"use strict";

describe("Tests suite for unit testing require.js global functions.", () => {
    beforeAll(() => {
        this._oldVoiceMyNews = global.voicemynews;
        this._oldRequire = global.require;
        this._requireInstance = jasmine.createSpyObj("RequireBinding", ["load", "loadRaw"]);

        this.__dirname = global.__dirname;
        this.__filename = global.__filename;

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
        _clearRequireCache();
    });

    afterEach(() => {
        _clearRequireCache();
        global.__filename = this.__filename;
        global.__dirname = this.__dirname;
        this._requireInstance.loadRaw.calls.reset();
    });

    afterAll(() => {
        global.voicemynews = this._oldVoiceMyNews;
        global.require = this._oldRequire;
    });

    it("require no cache no js extesion ok.", () => {
        testRequireTemplate("testModule", "testModule.js");
    });

    it("require no cache js extension specified.", () => {
        testRequireTemplate("testModule.js", "testModule.js");
    });

    it("require no cache no export ok.", () => {
        const dirName = "a/b";
        const moduleName = "testModule";
        const moduleSource = "";

        this._requireInstance.load.and.returnValue(moduleSource);

        let result = global.require(dirName + "/" + moduleName);

        expect(JSON.stringify(result)).toBe("{}");
        expect(this._requireInstance.load).toHaveBeenCalledWith(moduleName + ".js");
        expect(global.__filename).toBe(moduleName + ".js");
        expect(global.__dirname).toBe(dirName);
    });

    it("require no cache no export simple module name.", () => {
        const moduleName = "testModule";
        const moduleSource = "";

        this._requireInstance.load.and.returnValue(moduleSource);

        let result = global.require(moduleName);

        expect(JSON.stringify(result)).toBe("{}");
        expect(this._requireInstance.load).toHaveBeenCalledWith(moduleName + ".js");
        expect(global.__filename).toBe(moduleName + ".js");
        expect(global.__dirname).toBe("./");
    });

    it("require no cache explicit undefined export.", () => {
        const moduleName = "testModule";
        const moduleSource = "module.exports = undefined";

        this._requireInstance.load.and.returnValue(moduleSource);

        let result = global.require(moduleName);

        expect(result).toBe(undefined);
        expect(this._requireInstance.load).toHaveBeenCalledWith(moduleName + ".js");
    });

    it("require cache test.", () => {
        const expectedResult = testRequireTemplate("testModule", "testModule.js");
        const result = global.require("testModule");

        expect(expectedResult).toBe(result);
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

    /**
     * This function provides a template for testing require implementation.
     */
    let testRequireTemplate = (moduleName, expectedModuleName) => {
        const moduleSource = "module.exports = {id:1,id2:2}";

        this._requireInstance.load.and.returnValue(moduleSource);

        let result = global.require(moduleName);

        expect(result).not.toBe(undefined);
        expect(result.id).toBe(1);
        expect(result.id2).toBe(2);
        expect(this._requireInstance.load).toHaveBeenCalledWith(expectedModuleName);

        return result;
    }
});