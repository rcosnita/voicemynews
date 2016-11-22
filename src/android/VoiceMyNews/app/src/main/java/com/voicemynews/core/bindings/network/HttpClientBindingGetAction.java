package com.voicemynews.core.bindings.network;

/**
 * Provides the contract of a callback which can be used to be notified when a get request is complete.
 */
public class HttpClientBindingGetAction {
    /**
     * \brief Holds the pointer where we can find the js callback which must be invoked.
     */
    private long jsCallbackPtr;

    /**
     * \brief Holds the pointer where the current v8 isolate can be found.
     */
    private long isolatePtr;

    public HttpClientBindingGetAction(long jsCallbackPtr, long isolatePtr) {
        this.jsCallbackPtr = jsCallbackPtr;
        this.isolatePtr = isolatePtr;
    }

    /**
     * Provides the glue layer between java vm and v8 js engine.
     *
     * @param responseData The response data we wa
     */
    private native void invokeJsCallback(Object responseData);

    /**
     * This method is invoked by the http client get method when the action is complete. The method
     * receives the data from the get request.
     * @param responseData The response data received during get action.
     */
    public void onComplete(Object responseData) {
        invokeJsCallback(responseData);
    }
}
