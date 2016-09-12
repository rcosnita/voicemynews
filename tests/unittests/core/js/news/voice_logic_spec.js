"use strict";

const EventNames = require("js/events/event_names");
const voiceLogic = require("js/news/voice_logic");

const EventEmitter = require("events").EventEmitter;

describe("Tests suite for making sure voice reading business logic works as expected.", () => {
    beforeEach(() => {
        this._originalVoicemynews = global.voicemynews;
        this._buildEventData = jasmine.createSpy();
        this._eventLoop = new EventEmitter();
        this._voiceSupportBuilder = jasmine.createSpy();
        this._voiceSupport = jasmine.createSpyObj("VoiceSupport", ["readText", "readTextSsml"]);
        this._playerNotifications = undefined;

        this._voiceSupportBuilder.and.returnValue(this._voiceSupport);

        global.voicemynews = {
            core: {
                voice: {
                    VoiceSupport: {
                        getInstance: this._voiceSupportBuilder,
                        getNotificationsInstance: (whenProgress, whenPause, whenResume, whenDone) => {
                            this._playerNotifications = {
                                whenProgress: whenProgress,
                                whenPause: whenPause,
                                whenResume: whenResume,
                                whenDone: whenDone
                            }

                            return this._playerNotifications;
                        }
                    }
                }
            }
        };

        this._voiceLogic = new voiceLogic.VoiceLogic(this._eventLoop, this._buildEventData);
        expect(this._voiceSupportBuilder).toHaveBeenCalledWith();
        expect(this._playerNotifications).not.toBe(undefined);
        expect(this._playerNotifications.whenProgress).not.toBe(undefined);
        expect(this._playerNotifications.whenPause).not.toBe(undefined);
        expect(this._playerNotifications.whenResume).not.toBe(undefined);
        expect(this._playerNotifications.whenDone).not.toBe(undefined);
        expect(this._voiceLogic.pendingParagraphs).not.toBe(undefined);
        expect(this._voiceLogic.pendingParagraphs.length).toBe(0);
    });

    afterEach(() => {
        global.voicemynews = this._originalVoicemynews;
    });

    it("Test read news only headline ok.", () => {
        const newsModel = {
            "headline": "sample article",
            "paragraphs": []
        };

        this._voiceLogic.readNews(newsModel);
        expect(this._voiceSupport.readText).toHaveBeenCalledWith(newsModel.headline, this._playerNotifications);
        expect(this._voiceLogic.pendingParagraphs).not.toBe(undefined);
        expect(this._voiceLogic.pendingParagraphs.length).toBe(0);
    });

    it("Test read news headline and paragraphs ok.", () => {
        const newsModel = {
            "headline": "sample article",
            "paragraphs": [{
                "content": "paragraph 1"
            }, {
                "content": "paragraph 2"
            }]
        };

        this._voiceLogic.readNews(newsModel);
        expect(this._voiceSupport.readText).toHaveBeenCalledWith(newsModel.headline, this._playerNotifications);
        expect(this._voiceLogic.pendingParagraphs).not.toBe(undefined);
        expect(this._voiceLogic.pendingParagraphs.length).toBe(newsModel.paragraphs.length);
        expect(this._voiceLogic.pendingParagraphs[0]).toBe(newsModel.paragraphs[0]);
        expect(this._voiceLogic.pendingParagraphs[1]).toBe(newsModel.paragraphs[1]);
    });

    it("Test read news undefined news.", () => {
        this._voiceLogic.readNews();
        expect(this._voiceSupport.readText).not.toHaveBeenCalled();
        expect(this._voiceLogic.pendingParagraphs).not.toBe(undefined);
        expect(this._voiceLogic.pendingParagraphs.length).toBe(0);
    });

    it("Test read news no headline ok.", () => {
        this._voiceLogic.readNews({});
        expect(this._voiceSupport.readText).not.toHaveBeenCalled();
        expect(this._voiceLogic.pendingParagraphs).not.toBe(undefined);
        expect(this._voiceLogic.pendingParagraphs.length).toBe(0);
    });

    it("Test read news done continuation ok.", () => {
        const newsModel = {
            "headline": "sample article",
            "paragraphs": [{
                "content": "paragraph 1"
            }, {
                "content": "paragraph 2"
            }]
        };

        this._voiceLogic.readNews(newsModel);
        expect(this._voiceSupport.readText).toHaveBeenCalledWith(newsModel.headline, this._playerNotifications);

        this._playerNotifications.whenDone();
        expect(this._voiceSupport.readText).toHaveBeenCalledWith(newsModel.paragraphs[0].content, this._playerNotifications);
        expect(this._voiceLogic.pendingParagraphs).not.toBe(undefined);
        expect(this._voiceLogic.pendingParagraphs.length).toBe(1);
        expect(this._voiceLogic.pendingParagraphs[0]).toBe(newsModel.paragraphs[1]);

        this._playerNotifications.whenDone();
        expect(this._voiceSupport.readText).toHaveBeenCalledWith(newsModel.paragraphs[1].content, this._playerNotifications);
        expect(this._voiceLogic.pendingParagraphs).not.toBe(undefined);
        expect(this._voiceLogic.pendingParagraphs.length).toBe(0);
    });

    it("Test read news no remaining paragraph.", () => {
        this._playerNotifications.whenDone();
        expect(this._voiceSupport.readText).not.toHaveBeenCalled();
    });

    it("Test read news event handled ok.", () => {
           const newsModel = {
            "headline": "sample article",
            "paragraphs": [{
                "content": "paragraph 1"
            }, {
                "content": "paragraph 2"
            }]
        };

        this._voiceLogic.init();
        this._eventLoop.emit(EventNames.NEWS_VOICE_READ, {"evtData": JSON.stringify(newsModel)});
        expect(this._voiceSupport.readText).toHaveBeenCalledWith(newsModel.headline, this._playerNotifications);
        expect(this._voiceLogic.pendingParagraphs).not.toBe(undefined);
        expect(this._voiceLogic.pendingParagraphs.length).toBe(newsModel.paragraphs.length);
        expect(JSON.stringify(this._voiceLogic.pendingParagraphs[0])).toBe(JSON.stringify(newsModel.paragraphs[0]));
        expect(JSON.stringify(this._voiceLogic.pendingParagraphs[1])).toBe(JSON.stringify(newsModel.paragraphs[1]));
    });

    it("Test read news corrupted event ok.", () => {
        this._voiceLogic.init();
        
        this._eventLoop.emit(EventNames.NEWS_VOICE_READ, {"evtData": undefined});
        expect(this._voiceSupport.readText).not.toHaveBeenCalled();

        this._eventLoop.emit(EventNames.NEWS_VOICE_READ, {"evtData": ""});
        expect(this._voiceSupport.readText).not.toHaveBeenCalled();

        this._eventLoop.emit(EventNames.NEWS_VOICE_READ, {"evtData": "     "});
        expect(this._voiceSupport.readText).not.toHaveBeenCalled();
    });

    it("Test readProgress callback not implemented.", () => {
        try {
            this._playerNotifications.whenProgress(1);
            expect(true).toBeFalsy();
        } catch(err) {
            expect(err).not.toBe(undefined);
        }
    });

    it("Test readPaused callback not implemented.", () => {
        try {
            this._playerNotifications.whenPause(1);
            expect(true).toBeFalsy();
        } catch(err) {
            expect(err).not.toBe(undefined);
        }
    });

    it("Test readResumed callback not implemented.", () => {
        try {
            this._playerNotifications.whenResume(1);
            expect(true).toBeFalsy();
        } catch(err) {
            expect(err).not.toBe(undefined);
        }
    });
});