"use strict";

const EventEmitter = require("events").EventEmitter;
const EventNames = require("js/events/event_names");
const invalidPlayback = require("js/exceptions/invalid_playback");
const newsModel = require("js/news/news_datasource");
const Q = require("js/third_party/q/q");
const VoicePlaylistLogic = require("js/news/voice_playlist_logic");

describe("Tests suite for making sure voice playlist logic is correctly implemented.", () => {
    beforeEach(() => {
        this._eventLoop = new EventEmitter();
        this._buildEventData = jasmine.createSpy();
        this._voiceLogic = jasmine.createSpyObj("VoiceLogic", ["readNews", "pause", "resume", "skip"]);
        this._newsLogic = jasmine.createSpyObj("NewsLogic", ["fetchNewsByUrl"]);
        this._playlistLogic = VoicePlaylistLogic.init(this._eventLoop, this._buildEventData, this._voiceLogic,
            this._newsLogic);
        this._sampleEvtData = {
            "evtData": {
                "news": [{
                    "newsId": "http://samplehost.com/news1.html",
                    "provider": "cnn",
                    "rssDesc": {"attr1": "works"}
                }, {
                    "newsId": "http://samplehost2.com/news2.html",
                    "provider": "fox",
                    "rssDesc": {"attr1": "works"}
                }]
            }
        };
        this._sampleNewsModel1 = {
            "headline": "sample article",
            "paragraphs": []
        };
        this._sampleNewsModel2 = {
            "headline": "sample article",
            "paragraphs": [{
                "content": "paragraph 1"
            }, {
                "content": "paragraph 2"
            }]
        };

        expect(this._playlistLogic).not.toBe(undefined);
    });

    it("Read news works as expected.", (done) => {
        const evtData = this._sampleEvtData;
        const newsModel1 = this._sampleNewsModel1;
        const newsModel2 = this._sampleNewsModel2;

        let assertResultModel = (currArticle, expectedArticle, currPromise) => {
            expect(currArticle).toBe(expectedArticle);
            currPromise.resolve();
        }

        const readNewsNotifiers = [];
        this._voiceLogic.readNews.and.callFake((currArticle) => {
            const currNotifier = Q.defer();
            process.nextTick(() => {
                const currIdx = readNewsNotifiers.length;
                readNewsNotifiers.push(currNotifier);
                assertResultModel(currArticle, currIdx === 0 ? newsModel1 : newsModel2, currNotifier);
            });
            return currNotifier.promise;
        });

        _mockReadNews();

        this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST, {"evtData": JSON.stringify(evtData.evtData)});

        expect(this._playlistLogic.doneNotifier).not.toBe(undefined);

        this._playlistLogic.doneNotifier.then(() => {
            expect(this._voiceLogic.readNews).toHaveBeenCalledWith(newsModel2);
            expect(this._playlistLogic.doneNotifier).toBe(undefined);
            done();
        });
    });

    it("Read news while play in progress fails.", () => {
        const evtData = this._sampleEvtData;
        const evt = {"evtData": JSON.stringify(evtData.evtData)};
        this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST, evt);

        try {
            this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST, evt);
            expect(true).toBeFalsy("Method should have raised an exception ...");
        } catch(err) {
            expect(err instanceof invalidPlayback.MultiplePlaybackStreamsNotSupported).toBeTruthy();
        }
    });

    it("Pause news while play in progress ok.", (done) => {
        const evtData = this._sampleEvtData;
        const expectedPausedEvt = {};
        const pauseNotifier = Q.defer();
        let resolvePauseIdx = 0;

        this._buildEventData.and.returnValue(expectedPausedEvt);
        this._voiceLogic.pause.and.returnValue(pauseNotifier.promise);

        _mockReadNews(() => {
            if (resolvePauseIdx++ > 0) {
                return;
            }
            
            this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST_PAUSE, JSON.stringify({}));
            process.nextTick(() => {
                pauseNotifier.resolve();
            });
        });

        this._eventLoop.on(EventNames.NEWS_VOICE_READ_PLAYLIST_PAUSED, (pausedEvt) => {
            expect(this._buildEventData).toHaveBeenCalledWith("{}");
            expect(pausedEvt).toBe(expectedPausedEvt);
            expect(this._voiceLogic.pause).toHaveBeenCalledWith();
            done();
        });

        this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST, {"evtData": JSON.stringify(evtData.evtData)});
    });

    it("Pause news while play not in progress fails.", () => {
        try {
            this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST_PAUSE, JSON.stringify("{}"));
            expect(true).toBeFalsy();
        } catch(err) {
            expect(err instanceof invalidPlayback.PlaybackStreamNotPlaying).toBeTruthy();
            expect(err.message).toBe(invalidPlayback.PlaybackStreamNotPlaying.kDefaultMessage);
            expect(err.cause).toBe(invalidPlayback.PlaybackStreamNotPlaying.kDefaultCause);
            expect(err.stack).not.toBe(undefined);
        }
    });

    it("Resume news works ok.", (done) => {
        const evtData = this._sampleEvtData;
        const resumeResolver = Q.defer(); 
        const expectedEvt = {};

        this._buildEventData.and.returnValue(expectedEvt);
        this._voiceLogic.resume.and.returnValue(resumeResolver.promise);

        this._eventLoop.on(EventNames.NEWS_VOICE_READ_PLAYLIST_RESUMED, (resumedEvt) => {
            expect(this._buildEventData).toHaveBeenCalledWith("{}");
            expect(resumedEvt).toBe(expectedEvt);
            done();
        });

        this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST, {"evtData": JSON.stringify(evtData.evtData)});
        this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST_RESUME, {});

        expect(this._voiceLogic.resume).toHaveBeenCalled();
        process.nextTick(() => {
            resumeResolver.resolve();
        });
    });

    it("Resume news fails if no stream is playing.", () => {
        try {
            this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST_RESUME, {});
            expect(true).toBeFalsy();
        } catch(err) {
            expect(err instanceof invalidPlayback.PlaybackStreamNotPlaying);
            expect(err.message).toBe(invalidPlayback.PlaybackStreamNotPlaying.kDefaultMessage);
            expect(err.cause).toBe(invalidPlayback.PlaybackStreamNotPlaying.kDefaultCause);
            expect(err.stack).not.toBe(undefined);
        }

    });

    it("Skip news works as expected when stream is playing.", (done) => {
        const evtData = this._sampleEvtData;
        const pauseNotifier = Q.defer();
        const skipNotifier = Q.defer();
        const skippedEvt = {};
        const readNewsNotifier = Q.defer();
        let resolvePauseIdx = 0;

        _mockReadNews();

        this._buildEventData.and.returnValue(skippedEvt);
        this._voiceLogic.pause.and.callFake(() => {
            process.nextTick(() => {
                pauseNotifier.resolve();
            });

            return pauseNotifier.promise;
        });

        this._voiceLogic.readNews.and.callFake(() => {
            process.nextTick(() => {
                skipNotifier.resolve();
            });
            return readNewsNotifier.promise;
        });
        this._voiceLogic.skip.and.returnValue(skipNotifier.promise);

        this._eventLoop.on(EventNames.NEWS_VOICE_READ_PLAYLIST_SKIPPED, (evtData) => {
            expect(evtData).toBe(skippedEvt);
            expect(this._voiceLogic.pause).toHaveBeenCalled();
            expect(this._voiceLogic.skip).toHaveBeenCalled();
            expect(this._voiceLogic.readNews).toHaveBeenCalled();
            done();
        });

        this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST, {"evtData": JSON.stringify(evtData.evtData)});
        process.nextTick(() => this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST_SKIP, {}));
    });

    /**
     * Provides an elegant way to mock readNews behavior without copy and pasting boilerplate code.
     */
    let _mockReadNews = (fnMock) => {
        if (!fnMock) {
            fnMock = (url, newsProviderId, rssDesc, newsLoader) => {
                const evtData = this._sampleEvtData;
                const newsModel1 = this._sampleNewsModel1;
                const newsModel2 = this._sampleNewsModel2;

                if (url === evtData.evtData.news[0].newsId && newsProviderId === evtData.evtData.news[0].provider &&
                    JSON.stringify(rssDesc) === JSON.stringify(evtData.evtData.news[0].rssDesc)) {
                    newsLoader.resolve(newsModel1);
                    return;
                }
                else if (url === evtData.evtData.news[1].newsId && newsProviderId === evtData.evtData.news[1].provider &&
                    JSON.stringify(rssDesc) === JSON.stringify(evtData.evtData.news[1].rssDesc)) {
                    newsLoader.resolve(newsModel2);
                    return;
                }

                throw new Error("Not implemented ...");
            }
        }

        this._newsLogic.fetchNewsByUrl.and.callFake((url, newsProviderId, rssDesc) => {
            const newsLoader = Q.defer();

            process.nextTick(() => {
                fnMock(url, newsProviderId, rssDesc, newsLoader);
            });

            return newsLoader.promise;
        });
    }
});