"use strict";

const htmlhelper = require("js/news/datasources/html_helper");

describe("This tests suite ensures html helper utility module works as expected.", () => {
    it("getDomText works ok.", () => {
        const expectedResult = {
            type: "text",
            data: "sample text"
        }

        let dom = {
            children: [{
                    type: "unknown",
                    data: []
                }, expectedResult]
        }

        const results = htmlhelper.getDomText(dom);

        expect(results).not.toBe(undefined);
        expect(results.length).toBe(1);
        expect(results[0]).toBe(expectedResult.data);
    });

    it("getDomText dom with no text child returns empty.", () => {

    });

    it("getDomText undefined dom returns empty.", () => {
        const results = htmlhelper.getDomText(undefined);
        expect(results).not.toBe(undefined);
        expect(results.length).toBe(0);
    });

    it("getDomText null dom returns empty.", () => {
        const results = htmlhelper.getDomText(null);
        expect(results).not.toBe(undefined);
        expect(results.length).toBe(0);
    });

    it("getDomText dom with no children returns empty.", () => {
        const results = htmlhelper.getDomText({});
        expect(results).not.toBe(undefined);
        expect(results.length).toBe(0);
    });
});