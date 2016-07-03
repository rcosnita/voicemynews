"use strict";
((global) => {
    const RequireNative = new voicemynews.app.win10.bindings.RequireBinding();
    const loadedModulesCache = {};

    /**
     * This method provides an implementation of requirejs which is compatible with the nodejs.
     *
     * @param {String} moduleName the module we want to require as commonjs module. This is usually a relative path.
     * @returns {Object} an object which describes the public part of the module (exports).
     */
    global.require = (moduleName) => {
        if (!moduleName || moduleName.trim().length === 0) {
            return;
        }

        moduleName = enforceJsExt(moduleName);

        const cachedModule = getFromCache(moduleName);

        if (cachedModule) {
            return cachedModule.exports;
        }

        const moduleSource = RequireNative.load(moduleName);
        const exports = {};
        const module = {"exports": exports};
        const moduleObj = eval(moduleSource);

        loadedModulesCache[moduleName] = moduleObj;

        return module.exports;
    };

    /**
     * Using a syntax similir to require, this method loads the specified filename and returns the raw content.
     * At the moment, only text files are currently supported.
     *
     * @param {String} fileName the path to the file we want to load.
     * @returns {String} the file content.
     */
    global.requireRaw = (fileName) => {
        return RequireNative.loadRaw(fileName);
    };

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