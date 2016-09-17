"use strict";

const htmlhelper = require("js/news/datasources/html_helper");

describe("This tests suite ensures html helper utility module works as expected.", () => {
    it("getDomTag correctly extracts single tag without filters.", () => {
        const dom = [{
            "type": "unknown"
        }, {
            "type": "unknown"
        }, {
            "type": "tag",
            "name": "p"
        }];

        expect(htmlhelper.getDomTag(dom, "p")).toBe(dom[2]);
    });

    it("getDomTag correctly returns undefined when dom does not contain requested tag.", () => {
        const dom = [{
            "type": "unknown"
        }, {
            "type": "unknown"
        }];

        expect(htmlhelper.getDomTag(dom, "p")).toBe(undefined);
    });

    it("getDomTag correctly extracts many tags without filters.", () => {
        const dom = [{
            "type": "unknown"
        }, {
            "type": "unknown"
        }, {
            "type": "tag",
            "name": "p"
        }, {
            "type": "tag",
            "name": "p"
        }];

        const results = htmlhelper.getDomTag(dom, "p", undefined, true);
        expect(results).not.toBe(undefined);
        expect(results.length).toBe(2);
        expect(results[0]).toBe(dom[2]);
        expect(results[1]).toBe(dom[3]);
    });

    it("getDomTag correctly extracts single tag with filters.", () => {
        const dom = [{
            "type": "unknown"
        }, {
            "type": "unknown"
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {
                "data-attr": "1"
            }
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {
                "data-attr": "2"
            }
        }];

        expect(htmlhelper.getDomTag(dom, "p", {"data-attr": "2"})).toBe(dom[3]);
    });

    it("getDomTag correctly extracts single tag with multiple filters.", () => {
        const dom = [{
            "type": "unknown"
        }, {
            "type": "unknown"
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {
                "data-attr": "1"
            }
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {
                "data-attr": "1",
                "data-attr2": "1"
            }
        }];

        expect(htmlhelper.getDomTag(dom, "p", {"data-attr": "1", "data-attr2": "1"})).toBe(dom[3]);
    });

    it("getDomTag correctly extracts many tags with filters.", () => {
        const dom = [{
            "type": "unknown"
        }, {
            "type": "unknown"
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {
                "data-attr": "2"
            }
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {
                "data-attr": "2"
            }
        }];

        const results = htmlhelper.getDomTag(dom, "p", {"data-attr": "2"}, true);

        expect(results).not.toBe(undefined);
        expect(results.length).toBe(2);
        expect(results[0]).toBe(dom[2]);
        expect(results[1]).toBe(dom[3]);
    });

    it("getDomTag correctly extracts many tags with multiple filters.", () => {
        const dom = [{
            "type": "unknown"
        }, {
            "type": "unknown"
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {
                "data-attr": "1",
                "data-attr2": "1"
            }
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {
                "data-attr": "1",
                "data-attr2": "1"
            }
        }];

        const results = htmlhelper.getDomTag(dom, "p", {"data-attr": "1", "data-attr2": "1"}, true);
        expect(results).not.toBe(undefined);
        expect(results.length).toBe(2);
        expect(results[0]).toBe(dom[2]);
        expect(results[1]).toBe(dom[3]);
    });

    it("getDomTag correctly returns empty list when dom does not contain tags matching the given filter.", () => {
        const dom = [{
            "type": "unknown"
        }, {
            "type": "unknown"
        }];

        const results = htmlhelper.getDomTag(dom, "p", {"data-attr": "2"}, true);

        expect(results).not.toBe(undefined);
        expect(results.length).toBe(0);
    });

    it("getDomTag returns undefined when dom is undefined.", () => {
        expect(htmlhelper.getDomTag(undefined, "p")).toBe(undefined);
    });

    it("getDomTag returns undefined when dom is null.", () => {
        expect(htmlhelper.getDomTag(null, "p")).toBe(undefined);
    });

    it("getDomTag returns undefined when dom is empty list.", () => {
        expect(htmlhelper.getDomTag([], "p")).toBe(undefined);
    });

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

    it("getHeadingElements correctly extract h1 till h6 matching tags.", () => {
        const dom = {
            children: [{
                type: "tag",
                name: "h1",
                children: [{
                    "type": "text",
                    "data": "h1 tag"
                }]
            }, {
                type: "unknown",
                name: "custom"
            }, {
                type: "tag",
                name: "h2",
                children: [{
                    "type": "text",
                    "data": "h2 tag"
                }]
            }, {
                type: "unknown",
                name: "custom"
            }, {
                type: "tag",
                name: "h3",
                children: [{
                    "type": "text",
                    "data": "h3 tag"
                }]
            }, {
                type: "unknown",
                name: "custom"
            }, {
                type: "tag",
                name: "h4",
                children: [{
                    "type": "text",
                    "data": "h4 tag"
                }]
            }, {
                type: "unknown",
                name: "custom"
            }, {
                type: "tag",
                name: "h5",
                children: [{
                    "type": "text",
                    "data": "h5 tag"
                }]
            }, {
                type: "unknown",
                name: "custom"
            }, {
                type: "tag",
                name: "h6",
                children: [{
                    "type": "text",
                    "data": "h6 tag"
                }]
            }, {
                type: "tag",
                name: "h7",
                children: [{
                    "type": "text",
                    "data": "h7 tag"
                }]
            }]
        };

        let results = [];
        const success = htmlhelper.getHeadingElements(dom,
            (item, level) => results.push({
                "item": item,
                "level": level
            }));

        expect(success).toBeTruthy();
        expect(results.length).toBe(6);

        for (let idx = 0; idx < 6; idx++) {
            expect(results[idx].item).toBe("h" + (idx + 1) + " tag");
            expect(results[idx].level).toBe(idx + 1);
        }
    });

    it("getHeadingElements returns false if dom is undefined.", () => {
        expect(htmlhelper.getHeadingElements()).toBeFalsy();
    });

    it("getHeadingElements returns false if dom is null.", () => {
        expect(htmlhelper.getHeadingElements(null)).toBeFalsy();
    });

    it("getHeadingElements returns false if callback is undefined.", () => {
        const domWithH1 = {
            "children": [{
                "type": "tag",
                "name": "h1",
                "children": [{
                    "type": "text",
                    "data": "h1 tag"
                }]
            }]
        };

        expect(htmlhelper.getHeadingElements(domWithH1)).toBeFalsy();
    });

    it("getHeadingElements returns false if callback is null.", () => {
        const domWithH1 = {
            "children": [{
                "type": "tag",
                "name": "h1",
                "children": [{
                    "type": "text",
                    "data": "h1 tag"
                }]
            }]
        };

        expect(htmlhelper.getHeadingElements(domWithH1, null)).toBeFalsy();
    });

    it("getHeadingElements returns false if callback is not a function.", () => {
        const domWithH1 = {
            "children": [{
                "type": "tag",
                "name": "h1",
                "children": [{
                    "type": "text",
                    "data": "h1 tag"
                }]
            }]
        };

        expect(htmlhelper.getHeadingElements(domWithH1, 1)).toBeFalsy();
    });

    it("getTextContentFromTags works ok for single mode.", () => {
        const dom = [{
            "type": "unknown",
            "name": "custom-tag"
        }, {
            "type": "tag",
            "name": "p",
            "children": [{
                "type": "text",
                "data": "p tag."
            }]
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {"data-attr": "1"},
            "children": [{
                "type": "text",
                "data": "p tag second."
            }]
        }];

        const extractedText = [];
        const result = htmlhelper.getTextContentFromTags(dom, "p", {"data-attr": "1"}, false,
            (text) => extractedText.push(text));

        expect(result).toBe(dom[2]);
        expect(extractedText.length).toBe(1);
        expect(extractedText[0]).toBe("p tag second.");
    });

    it("getTextContentFromTags works ok for many mode.", () => {
        const dom = [{
            "type": "unknown",
            "name": "custom-tag"
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {"data-attr": "1"},
            "children": [{
                "type": "text",
                "data": "p tag."
            }]
        }, {
            "type": "tag",
            "name": "p",
            "attribs": {"data-attr": "1"},
            "children": [{
                "type": "text",
                "data": "p tag."
            }]
        }];

        const extractedText = [];
        const results = htmlhelper.getTextContentFromTags(dom, "p", {"data-attr": "1"}, true,
            (text) => extractedText.push(text));

        expect(results).not.toBe(undefined);
        expect(results.length).toBe(2);
        expect(results[0]).toBe(dom[1]);
        expect(results[1]).toBe(dom[2]);
        expect(extractedText.length).toBe(2);
        expect(extractedText[0]).toBe("p tag.");
        expect(extractedText[1]).toBe("p tag.");
    });

    it("getTextContentFromTags returns empty list for undefined dom.", () => {
        expect(htmlhelper.getTextContentFromTags().length).toBe(0);
    });

    it("getTextContentFromTags returns empty list for null dom.", () => {
        expect(htmlhelper.getTextContentFromTags(null).length).toBe(0);
    });

    it("getTextContentFromTags returns empty list for undefined callback.", () => {
        const dom = [{
            "type": "tag",
            "name": "p"
        }];

        expect(htmlhelper.getTextContentFromTags(dom, "p", undefined, false).length).toBe(0);
    });

    it("getTextContentFromTags returns empty list for null callback.", () => {
        const dom = [{
            "type": "tag",
            "name": "p"
        }];

        expect(htmlhelper.getTextContentFromTags(dom, "p", undefined, false, null).length).toBe(0);
    });

    it("getTextContentFromTags returns empty list for not a function callback.", () => {
        const dom = [{
            "type": "tag",
            "name": "p"
        }];

        expect(htmlhelper.getTextContentFromTags(dom, "p", undefined, false, 1).length).toBe(0);
    });
});