"use strict";

const EventEmitter = require("events").EventEmitter;
const EventNames = require("js/events/event_names");
const newsModel = require("js/news/news_datasource");
const Q = require("js/third_party/q/q");
const VoicePlaylistLogic = require("js/news/voice_playlist_logic");

describe("Tests suite for making sure voice playlist logic is correctly implemented.", () => {
    beforeEach(() => {
        this._eventLoop = new EventEmitter();
        this._buildEventData = jasmine.createSpy();
        this._voiceLogic = jasmine.createSpyObj("VoiceLogic", ["readNews", "pause", "resume"]);
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

    it("Read news work as expected.", (done) => {
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
            setTimeout(() => {
                const currIdx = readNewsNotifiers.length;
                readNewsNotifiers.push(currNotifier);
                assertResultModel(currArticle, currIdx === 0 ? newsModel1 : newsModel2, currNotifier);
            });
            return currNotifier.promise;
        });

        this._newsLogic.fetchNewsByUrl.and.callFake((url, newsProviderId, rssDesc) => {
            const newsLoader = Q.defer();

            setTimeout(() => {
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
            });

            return newsLoader.promise;
        });

        this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST, JSON.stringify(evtData));

        expect(this._playlistLogic.doneNotifier).not.toBe(undefined);

        this._playlistLogic.doneNotifier.then(() => {
            expect(this._voiceLogic.readNews).toHaveBeenCalledWith(newsModel2);
            expect(this._playlistLogic.doneNotifier).toBe(undefined);
            done();
        });
    });
});