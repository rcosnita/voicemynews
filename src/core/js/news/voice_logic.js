/**
 * This module provides the logic for aplying text to speech algorithm to a given paragraph.
 * It is designed so that it works in foreground and background without any code changes. The main assumption
 * is C++ native bindings which provides the TTS native support.
 *
 * @module voicemynews/js/news/voice_logic
 */
"use strict";

const EventNames = require("js/events/event_names");
const AnalyticsConstants = require("js/analytics/analytics_constants");
const invalidPlayback = require("js/exceptions/invalid_playback");
const NotImplementedMethodException = require("js/exceptions/notimplemented").NotImplementedMethodException;
const Q = require("js/third_party/q/q");

/**
 * This class provides all the logic for transforming a news model into audio and internally relies on platform
 * specific TTS provider.
 *
 * @alias module:voicemynews/js/news/voice_logic.VoiceLogic
 */
class VoiceLogic {
    constructor(eventLoop, buildEventData, analyticsLogic) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
        this._analyticsLogic = analyticsLogic;
        this._voiceSupport = voicemynews.core.voice.VoiceSupport.getInstance();
        this._remainingParagraphs = [];
        this._playerNotifications = voicemynews.core.voice.VoiceSupport.getNotificationsInstance(
            (currPos) => this._whenParagraphReadInProgress(currPos),
            (currPos) => this._whenParagraphReadPaused(currPos),
            (currPos) => this._whenParagraphReadResumed(currPos),
            () => this._whenParagraphReadDone()
        );

        this._doneNotifier = undefined;
        this._pauseNotifier = undefined;
        this._resumeNotifier = undefined;
    }

    /**
     * Obtains current pending paragraphs which are going to be read by voice logic.
     * @property
     */
    get pendingParagraphs() {
        return this._remainingParagraphs;
    }

    /**
     * Obtains current stream done notifier. In case no stream is playing this will return undefined.
     * @property
     */
    get doneNotifier() {
        return !this._doneNotifier ? undefined : this._doneNotifier.promise;
    }

    /**
     * Wires all relevant event handlers so that voice logic can collaborate with the native side.
     *
     * @public
     * @method
     */
    init() {
        this._eventLoop.on(EventNames.NEWS_VOICE_READ, (evt) => this._handleReadNews(evt));
    }

    /**
     * Pause the current reading stream. It returns a promise which resolves once the pause
     * action has been executed.
     */
    pause() {
        if (this._pauseNotifier) {
            throw new invalidPlayback.PlaybackStreamNotPlaying();
        }

        this._pauseNotifier = Q.defer();
        try {
            return this._pauseNotifier.promise;
        } finally {
            this._voiceSupport.pause(this._playerNotifications);
        }
    }

    /**
     * Provides the algorithm for transforming the given news model into voice reading. Internally, it separates
     * paragraphs and empirically chooses the right amount of text which must be output to the audio device.
     * Moreover, the method provides an advanced algorithm for indicating the user the presence of images and
     * videos found in the news.
     *
     * @public
     * @method
     * @param {module:voicemynews/js/news/news_datasource.NewsModel} newsModel the news model which must be transformed
     * to audio.
     */
    readNews(newsModel) {
        if (!newsModel || !newsModel.headline) {
            return;
        }

        this._doneNotifier = Q.defer();

        this._remainingParagraphs = newsModel.paragraphs;
        this._voiceSupport.readText(newsModel.headline, this._playerNotifications);

        return this._doneNotifier.promise;
    }

    /**
     * Provides the algorithm for resuming playing the current loaded stream.
     */
    resume() {
        this._resumeNotifier = Q.defer();

        try {
            return this._resumeNotifier.promise;
        } finally {
            this._voiceSupport.resume(this._playerNotifications);
        }
    }

    /**
     * Provides the algorithm for skipping the current playback stream.
     */
    skip() {
        const skipNotifier = Q.defer();

        try {
            return skipNotifier.promise;
        } finally {
            this._voiceSupport.skip();
            this._doneNotifier.resolve();
            skipNotifier.resolve();
        }
    }

    /**
     * Provides the logic for transforming the given array of paragraphs into audio.
     *
     * @private
     * @method
     * @param {Array} paragraphs the paragraphs which must be output to the audio device. See {@link module:voicemynews/js/news/news_datasource.NewsParagraphModel} for more information.
     */
    _readParagraphs(paragraphs) {
        if (!paragraphs || paragraphs.length === 0) {
            if (this._doneNotifier) {
                this._doneNotifier.resolve();
                this._doneNotifier = undefined;
            }

            return;
        }

        const currParagraph = paragraphs[0]
        this._remainingParagraphs = paragraphs.slice(1, paragraphs.length);

        this._voiceSupport.readText(currParagraph.content, this._playerNotifications);
    }

    /**
     * Handles the read news with voice event received from native part of the app.
     *
     * @private
     * @method
     * @param {Event} evt a native event which provides access to evtData property.
     */
    _handleReadNews(evt) {
        if (!evt.evtData || evt.evtData.trim().length === 0) {
            return;
        }

        this._analyticsLogic.logEvent({
            eventCategory: AnalyticsConstants.categories.JS_INDIVIDUALNEWS_ONSCREEN,
            eventAction: AnalyticsConstants.events.JS_READ_ACTION,
            eventLabel: AnalyticsConstants.labels.JS_READ_LABEL,
            eventValue: 1
        });

        const newsModel = JSON.parse(evt.evtData);
        this.readNews(newsModel);
    }

    /**
     * Handles read in progress events coming from voice support.
     *
     * @param {Number} currPos the current position inside the reading stream.
     */
    _whenParagraphReadInProgress(currPos) {
        throw new NotImplementedMethodException();
    }

    /**
     * Handles read paused events coming from voice support.
     *
     * @param {Number} currPos the current position inside the reading stream.
     */
    _whenParagraphReadPaused(currPos) {
        this._pauseNotifier.resolve();
        this._pauseNotifier = undefined;
    }

    /**
     * Handles read resumed events coming from voice support.
     *
     * @param {Number} currPos the current position inside the reading stream.
     */
    _whenParagraphReadResumed(currPos) {
        this._resumeNotifier.resolve();
        this._resumeNotifier = undefined;
    }

    /**
     * Handles read completed events coming from voice support.
     */
    _whenParagraphReadDone() {
        this._readParagraphs(this._remainingParagraphs);
    }
}

module.exports = {
    VoiceLogic: VoiceLogic,
    init: (eventLoop, buildEventData, analyticsLogic) => {
        let voiceLogic = new VoiceLogic(eventLoop, buildEventData, analyticsLogic);
        voiceLogic.init();
        return voiceLogic;
    }
};