/**
 * This module provides the logic for aplying text to speech algorithm to a given paragraph.
 * It is designed so that it works in foreground and background without any code changes. The main assumption
 * is C++ native bindings which provides the TTS native support.
 *
 * @module voicemynews/js/news/voice_logic
 */
"use strict";

const EventNames = require("js/events/event_names");

/**
 * This class provides all the logic for transforming a news model into audio and internally relies on platform
 * specific TTS provider.
 *
 * @alias module:voicemynews/js/news/voice_logic.VoiceLogic
 */
class VoiceLogic {
    constructor(eventLoop, buildEventData) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
        this._voiceBinding = new voicemynews.app.win10.bindings.news.VoiceBinding();
        this._remainingParagraphs = [];
        this._playerNotifications = new voicemynews.app.win10.bindings.news.VoiceNotifications(
            () => this._whenParagraphRead()
        );
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
        this._remainingPagraphs = newsModel.paragraphs;
        this._voiceBinding.readText(newsModel.headline, this._playerNotifications);
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
            return;
        }

        const currParagraph = paragraphs[0]
        this._remainingPagraphs = paragraphs.slice(1, paragraphs.length);

        this._voiceBinding.readText(currParagraph.content, this._playerNotifications);
    }

    /**
     * Handles the read news with voice event received from native part of the app.
     *
     * @private
     * @method
     * @param {Event} evt a native event which provides access to evtData property.
     */
    _handleReadNews(evt) {
        const newsModel = JSON.parse(evt.evtData);

        this.readNews(newsModel);
    }

    _whenParagraphRead() {
        this._readParagraphs(this._remainingPagraphs);
    }
}

module.exports = {
    VoiceLogic: VoiceLogic,
    init: (eventLoop, buildEventData) => {
        let voiceLogic = new VoiceLogic(eventLoop, buildEventData);
        voiceLogic.init();
        return voiceLogic;
    }
};