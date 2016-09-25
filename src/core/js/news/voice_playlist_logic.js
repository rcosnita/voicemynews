/**
 * This module provides the logic for aplying text to speech algorithm to a given paragraph.
 * It is designed so that it works in foreground and background without any code changes. The main assumption
 * is C++ native bindings which provides the TTS native support.
 *
 * @module voicemynews/js/news/voice_playlist_logic
 */
"use strict";

/**
 * Provides the algorithm for reading a subset of news coming from genius news. Internally, it received a reading
 * event with all the urls which must be read sequentially and then starts fetching the news model and read them.
 * The number of requests is optimized so that end user benefits from the best possible experience.
 *
 * @alias module:voicemynews/js/news/voice_playlist_logic.VoicePlaylistLogic
 */
class VoicePlaylistLogic {
    constructor(eventLoop, buildEventData, voiceLogic) {
        this._eventLoop = eventLoop;
        this._buildEventData = buildEventData;
        this._voiceLogic = voiceLogic;
    }

    /**
     * Wires all events to the given event loop in order to correctly expose playlist reading functionality.
     *
     * @public
     * @method
     */
    init() {

    }
}

module.exports = {
    VoicePlaylistLogic: VoicePlaylistLogic,
    init: (eventLoop, buildEventData, voiceLogic) => {
        const voicePlaylistLogic = new VoicePlaylistLogic(eventLoop, buildEventData, voiceLogic);
        voicePlaylistLogic.init();
        return voicePlaylistLogic;
    }
}