#ifndef VoiceMyNewsAppUI_win10_bindings_network_HttpClientBinding_H_
#define VoiceMyNewsAppUI_win10_bindings_network_HttpClientBinding_H_

#include "network/HttpClientInterface.h"
#include "network/HttpClientNativeWin.h"
#include "JsApp.h"

#include <collection.h>

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
using Windows::Foundation::Collections::IMap;
using Platform::String;
using Windows::Foundation::IAsyncOperation;
using Windows::Foundation::IAsyncOperationWithProgress;
using Windows::Foundation::Uri;
using Windows::Web::Http::Headers::HttpContentHeaderCollection;
using Windows::Web::Http::HttpProgress;
using Windows::Web::Http::HttpRequestMessage;
using Windows::Web::Http::HttpResponseMessage;

/**
 * \class HttpResponseMessageParserBinding
 * \brief This class provides the data structure for a parsed response.
 */
public ref class HttpResponseMessageParsed sealed {
public:
    HttpResponseMessageParsed(int statusCode, String^ reason, HttpContentHeaderCollection^ headers, String^ content);

    /**
     * \brief This method obtains the status code from the current response.
     */
    int GetStatusCode();

    /**
     * \brief This method obtains the reason accompanying the status code.
     */
    String^ GetReason();

    /**
     * \brief This method obtains the content from the current response.
     */
    String^ GetContent();

    /**
     * \brief This method obtains the response headers from the current response.
     */
    HttpContentHeaderCollection^  GetHeaders();
private:
    int statusCode_;
    String^ reason_;
    String^ content_;
    HttpContentHeaderCollection^ headers_;
};

/**
 * \brief Provides the callback api which can be used to receive notification when a request is successful.
 */
public delegate void HttpClientBindingRequestOnSuccess(HttpResponseMessage^ responseMessage);

/**
 * \brief Provides the callback api which can be used to receive the parsed message of a http call when ready.
 */
public delegate void HttpClientBindingRequestOnParsed(HttpResponseMessageParsed^ messageParsed);

/**
 * \class HttpClientWin
 * \brief This class provides the http client specific implementation for windows.
 *
 * The class relies the implementation on Windows::Web WinRT components and it can be automatically proejecte.
 */
public ref class HttpClientBinding sealed {
using EventLoopBinding = voicemynews::app::win10::bindings::events::EventLoopBinding;
using HeadersStorage = Windows::Foundation::Collections::IMap<Platform::String^, Platform::String^>;
public:
    HttpClientBinding();

    /**
     * \brief This method provides an implementation for http get which is async.
     *
     * \param uri the uri we want to get with this request.
     * \param requestHeaders the request headers we want to send with the request.
     * \returns a promise which receives the http response message. It is a good idea to parse it using the helper
     * methods provided by this class.
     */
    IAsyncOperationWithProgress<HttpResponseMessage^, HttpProgress>^ Get(String^ uri, IMap<String^, String^>^ requestHeaders);

    /**
     * \brief This method provides an implementation for http get which is async and executes the given callback on js thread.
     * JavaScript business logic must always use this method in order to do get requests.
     *
     * \param uri the uri we want to get with this request.
     * \param requestHeaders the request headers we want to send with the request.
     * \param onSuccess the success callback which must be invoked for the http request.
     */
    void Get(String^ uri, HeadersStorage^ requestHeaders, HttpClientBindingRequestOnSuccess^ onSuccess);

    /**
     * \brief This method provides a helper method for parsing message and fetching the response payload as string.
     *
     * \param msg The message we want to parse.
     */
    IAsyncOperation<HttpResponseMessageParsed^>^ ParseResponseWithStringContent(HttpResponseMessage^ msg);

    /**
     * \brief This method provides a helper for parsing the response message object received after an http request.
     * It guarantees that the given callback is executed on js thread.
     *
     * \param msg The response message received after a http request completed.
     * \param onParsed The callback we want to executed once the message parsing is complete,
     */
    void ParseResponseWithStringContent(HttpResponseMessage^ msg, HttpClientBindingRequestOnParsed^ onParsed);

    /**
     * \brief Intantiate a new map which can be used as headers storage.
     *
     * \returns the map instance which can store headers.
     */
    HeadersStorage^ GetNewHeadersMap();

private:
    /**
     * \brief This method copies the provided headers into request message.
     */
    void CopyHeadersToRequestMessage(IMap<String^, String^>% headers, HttpRequestMessage% requestMessage);

private:
    EventLoopBinding^ jsLoop_;
};
}
}
}
}

namespace voicemynews {
namespace core {
namespace network {
using voicemynews::app::win10::bindings::HttpClientBinding;

/**
 * \brief This class provides the factory implementation for obtaining a platform specific instance of HttpClient.
 */
public ref class HttpClient sealed {
public:
    static HttpClientBinding^ GetInstance();
};
}
}
}

#endif // VoiceMyNewsCore_win10_bindings_network_HttpClientBinding_H_