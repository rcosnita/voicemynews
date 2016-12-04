package com.voicemynews.core.bindings.network;

/**
 * Provides the descriptor of a http string response.
 */
public class HttpClientBindingParsedStringResponse extends HttpClientBindingParsedResponseAbstract<String> {
    public HttpClientBindingParsedStringResponse(int statusCode, String response) {
        super(statusCode, response);
    }
}
