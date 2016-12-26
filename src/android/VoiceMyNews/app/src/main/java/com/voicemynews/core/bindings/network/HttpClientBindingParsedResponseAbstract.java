package com.voicemynews.core.bindings.network;

/**
 * Provides the class which describes an http response accurately.
 */
public abstract class HttpClientBindingParsedResponseAbstract<T> {
    private final int statusCode;
    private final T content;

    public int getStatusCode() {
        return statusCode;
    }

    public T getContent() {
        return content;
    }


    public HttpClientBindingParsedResponseAbstract(int statusCode, T content) {
        this.statusCode = statusCode;
        this.content = content;
    }
}
