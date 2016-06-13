/*
 * This script provides a helper for creating the correct structure of folder under node_modules
 * required for unit tests to run. Internally, the script does the following:
 *
 * 1. copy **core/js** folder under node_modules.
 * 1. creates **node_modules/js/third_party** symbolic link which points to node_modules. 
 */
"use strict";

const fs = require("fs-extra");
const path = require("path");

const jsSourceCode = path.join("src", "core", "js");
const jsThirdParty = path.join("node_modules", "js", "third_party")
const jsFolder = path.join("node_modules", "js");

fs.removeSync(jsFolder);
fs.copySync(jsSourceCode, jsFolder);
fs.ensureSymlink("node_modules", jsThirdParty);