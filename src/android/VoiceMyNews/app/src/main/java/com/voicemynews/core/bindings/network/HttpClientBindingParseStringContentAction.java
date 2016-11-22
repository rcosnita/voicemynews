package com.voicemynews.core.bindings.network;

/**
 * Provides the model for an action which can be invoked after a request body is completely parsed as
 * string content.
 */
public class HttpClientBindingParseStringContentAction {
    /**
     * Holds the pointer to the javascript callback method which must be invoked.
     */
    private long jsCallbackPtr;

    /**
     * Holds the pointer to the v8 engine where the action took place.
     */
    private long isolatePtr;

    public HttpClientBindingParseStringContentAction(long jsCallbackPtr, long isolatePtr) {
        this.jsCallbackPtr = jsCallbackPtr;
        this.isolatePtr = isolatePtr;
    }

    /**
     * Provides the glue layer between java vm and v8 engine. It invokes the binded javascript callback.
     * @param content
     */
    private native void invokeJsCallback(String content);

    /**
     * The method invoked once the request content has been parsed.
     * @param content The content parsed as raw string.
     */
    public void onParsed(String content)
    {
        invokeJsCallback(content);
    }
}
