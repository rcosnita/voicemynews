"use strict";
((global) => {
    const RequireNative = new voicemynews.app.win10.bindings.RequireBinding();

    global.require = (moduleName) => {
        const moduleSource = RequireNative.load(moduleName);
        const exports = {};
        const module = {"exports": exports};
        const moduleObj = eval(moduleSource);

        return module.exports;
    };
})(this);