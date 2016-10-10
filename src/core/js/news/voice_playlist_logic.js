/**
 * This module provides the logic for aplying text to speech algorithm to a given paragraph.
 * It is designed so that it works in foreground and background without any code changes. The main assumption
 * is C++ native bindings which provides the TTS native support.
 *
 * @module voicemynews/js/news/voice_playlist_logic
 */
"use strict";

const EventNames = require("js/events/event_names");
const invalidPlayback = require("js/exceptions/invalid_playback");
const Q = require("js/third_party/q/q");

/**
 * Provides the algorithm for reading a subset of news coming from genius news. Internally, it received a reading
 * event with all the urls which must be read sequentially and then starts fetching the news model and read them.
 * The number of requests is optimized so that end user benefits from the best possible experience.
 *
 * @alias module:voicemynews/js/news/voice_playlist_logic.VoicePlaylistLogic
 */
class VoicePlaylistLogic {
    constructor(eventLoop, buildEventData, voiceLogic, newsLogic) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
        this._voiceLogic = voiceLogic;
        this._newsLogic = newsLogic;
        this._doneNotifier = undefined;
    }

    /**
     * Provides information about the current reading session. The result can be undefined if no session is in
     * progress.
     * @property doneNotifier
     */
    get doneNotifier() {
        return this._doneNotifier ? this._doneNotifier.promise : undefined;
    }

    /**
     * Wires all events to the given event loop in order to correctly expose playlist reading functionality.
     *
     * @public
     * @method
     */
    init() {
        this._eventLoop.on(EventNames.NEWS_VOICE_READ_PLAYLIST, (evt) => this._handleReadPlaylist(evt));
        this._eventLoop.on(EventNames.NEWS_VOICE_READ_PLAYLIST_PAUSE, (evt) => this._handlePausePlaylist());
        this._eventLoop.on(EventNames.NEWS_VOICE_READ_PLAYLIST_RESUME, (evt) => this._handleResumePlaylist());
        this._eventLoop.on(EventNames.NEWS_VOICE_READ_PLAYLIST_SKIP, (evt) => this._handleSkipPlaylistNews());
    }

    /**
     * Reads all given news in sequential order. Internally it relies on voice logic in order to provide reading
     * functionality.
     *
     * @private
     * @method
     * @param {Array} news an array of news model which can be fetched from specific providers and read.
     * @returns {Promise} a promise which can be used to determine when all the news were read.
     */
    _readAllNews(news) {
        if (this._doneNotifier) {
            throw new invalidPlayback.MultiplePlaybackStreamsNotSupported();
        }

        this._doneNotifier = Q.defer();
        const articleLoaders = [];
        const articles = [];

        news.forEach((newsDesc) => {
            let newsLoader = this._newsLogic.fetchNewsByUrl(newsDesc.newsId, newsDesc.provider, newsDesc.rssDesc);
            articleLoaders.push(newsLoader);
        });

        Q.all(articleLoaders).then(() => {
            articleLoaders.forEach((article) => articles.push(article));
            this._readArticleAndContinue(articles, 0, () => {
                this._doneNotifier.resolve();
                this._doneNotifier = undefined;
            });
        });
    }

    /**
     * Pauses the voice reading of the current playlist.
     */
    _handlePausePlaylist() {
        if (!this.doneNotifier) {
            throw new invalidPlayback.PlaybackStreamNotPlaying();
        }

        const pauseEvt = this._buildEventData("{}");
        this._voiceLogic.pause().then(() => {
            this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST_PAUSED, pauseEvt);
        });
    }

    /**
     * Parses the event received from native side and starts reading the playlist described in the event.
     *
     * @private
     * @method
     */
    _handleReadPlaylist(evt) {
        const evtData = JSON.parse(evt.evtData);
        this._readAllNews(evtData.news);
    }

    /**
     * Resumes reading the current news playlist stream.
     */
    _handleResumePlaylist() {
        if (!this.doneNotifier) {
            throw new invalidPlayback.PlaybackStreamNotPlaying();
        }

        const evt = this._buildEventData("{}");
        this._voiceLogic.resume().then(() => {
            this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST_RESUMED, evt);
        });
    }

    /**
     * Skips the current news which is read by TTS. Once everything is done it emits an event
     * telling the native part that skip operation was executed.
     */
    _handleSkipPlaylistNews() {
        if (!this._doneNotifier) {
            throw new invalidPlayback.PlaybackStreamNotPlaying();
        }

        this._voiceLogic.pause().then(() => {
            this._voiceLogic.skip().then(() => {
                const evtData = this._buildEventData("{}");
                this._eventLoop.emit(EventNames.NEWS_VOICE_READ_PLAYLIST_SKIPPED, evtData);
            });
        });
    }

    /**
     * Provides the logic for reading an individual article found in articles array. Once the reading is done,
     * it moves on to the next article.
     *
     * @private
     * @method
     */
    _readArticleAndContinue(articles, articleIdx, whenFinished) {
        const currArticle = articles[articleIdx];
        this._voiceLogic.readNews(currArticle).then(() => {
            articleIdx++;
            if (articleIdx < articles.length) {
                this._readArticleAndContinue(articles, articleIdx, whenFinished);
                return;
            }

            whenFinished();
        });
    }
}

module.exports = {
    VoicePlaylistLogic: VoicePlaylistLogic,
    init: (eventLoop, buildEventData, voiceLogic, newsLogic) => {
        const voicePlaylistLogic = new VoicePlaylistLogic(eventLoop, buildEventData, voiceLogic, newsLogic);
        voicePlaylistLogic.init();
        return voicePlaylistLogic;
    }
}