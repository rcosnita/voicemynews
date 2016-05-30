"use strict";
((global) => {
    const RequireNative = new voicemynews.app.win10.bindings.RequireBinding();

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

        const moduleSource = RequireNative.load(moduleName);
        const exports = {};
        const module = {"exports": exports};
        const moduleObj = eval(moduleSource);

        return module.exports;
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
    }
})(this);