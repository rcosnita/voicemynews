"use strict";
((globalCtx) => {
    if (Object.keys(globalCtx).length === 0) {
        globalCtx = global;
    }

    const RequireNative = voicemynews.core.RequireFactory.getInstance();
    var loadedModulesCache = {};

    /**
     * This method provides an implementation of requirejs which is compatible with the nodejs.
     *
     * @param {String} moduleName the module we want to require as commonjs module. This is usually a relative path.
     * @returns {Object} an object which describes the public part of the module (exports).
     */
    globalCtx.require = (moduleName) => {
        if (!moduleName || moduleName.trim().length === 0) {
            return;
        }

        moduleName = enforceJsExt(moduleName);
        setNodeEnvironment(moduleName);

        const cachedModule = getFromCache(moduleName);

        if (cachedModule) {
            return cachedModule.exports;
        }

        const moduleSource = RequireNative.load(moduleName);
        const exports = {};
        const module = {"exports": exports};
        const moduleObj = eval(moduleSource);

        loadedModulesCache[moduleName] = module;

        return module.exports;
    };

    if (!globalCtx.setTimeout) {
        /**
         * This method provides a shim for setTimeout JS method in case it is not provided by the js engine.
         * At the moment, if delay is defined and greater than 0 an exception is raised.
         */
        globalCtx.setTimeout = (fn, delay) => {
            if (delay && delay > 0) {
                throw new Error("Delay not currently supported ...");
            }

            fn();
        };
    }

    /**
     * Using a syntax similir to require, this method loads the specified filename and returns the raw content.
     * At the moment, only text files are currently supported.
     *
     * @param {String} fileName the path to the file we want to load.
     * @returns {String} the file content.
     */
    globalCtx.requireRaw = (fileName) => {
        return RequireNative.loadRaw(fileName);
    };

    /**
     * This method provides a way to clear internal require cache. It should be used only in unit tests.
     */
    globalCtx._clearRequireCache = () => {
        loadedModulesCache = {};
    }

    /**
     * This method enforces the given module name to end with .js.
     *
     * @private
     * @method
     * @param {String} moduleName the name of the module for which we want to enforce .js extension.
     * @returns {String} the new moduleName with .js extension included.
     */
    let enforceJsExt = (moduleName) => {
        if (moduleName.lastIndexOf(".js") !== moduleName.length - ".js".length) {
            moduleName += ".js";
        }

        return moduleName;
    };

    /**
     * This method sets all required global variables required to simulate node.
     *
     * @param {String} moduleName the moduleName from which we infer __filename and __dirname.
     */
    let setNodeEnvironment = (moduleName) => {
        let slashPos = moduleName.lastIndexOf("/");

        if (slashPos < 0) {
            globalCtx.__dirname = "./";
            globalCtx.__filename = moduleName;
            return;
        }

        globalCtx.__dirname = moduleName.substr(0, slashPos);
        globalCtx.__filename = moduleName.substr(slashPos + 1);
    };

    /**
     * This method tries to load the given module name from module cache.
     *
     * @private
     * @method
     * @param {String} moduleName the name of the module which we want to load from cache.
     * @returns {Object} the cached version of the module or undefined if the module was not previously loaded.
     */
    let getFromCache = (moduleName) => {
        return loadedModulesCache[moduleName];
    };
})(this);