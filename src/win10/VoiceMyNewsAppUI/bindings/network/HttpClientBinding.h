#ifndef VoiceMyNewsAppUI_win10_bindings_network_HttpClientBinding_H_
#define VoiceMyNewsAppUI_win10_bindings_network_HttpClientBinding_H_

#include "network/HttpClientInterface.h"
#include "network/HttpClientNativeWin.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
using Platform::String;
using Windows::Foundation::IAsyncOperation;
using Windows::Foundation::IAsyncOperationWithProgress;
using Windows::Foundation::Uri;
using Windows::Web::Http::HttpProgress;
using Windows::Web::Http::Headers::HttpResponseHeaderCollection;
using Windows::Web::Http::HttpResponseMessage;

/**
 * \class HttpResponseMessageParserBinding
 * \brief This class provides the data structure for a parsed response.
 */
public ref class HttpResponseMessageParsed sealed {
public:
    HttpResponseMessageParsed(int statusCode, String^ reason, HttpResponseHeaderCollection^ headers, String^ content);

    int GetStatusCode();

    String^ GetReason();

    String^ GetContent();

    HttpResponseHeaderCollection^  GetHeaders();
private:
    int statusCode_;
    String^ reason_;
    String^ content_;
    HttpResponseHeaderCollection^ headers_;
};

/**
 * \class HttpClientWin
 * \brief This class provides the http client specific implementation for windows.
 *
 * The class relies the implementation on Windows::Web WinRT components and it can be automatically proejecte.
 */
public ref class HttpClientBinding sealed {
public:
    /**
     * \brief This method provides an implementation for http get which is async.
     *
     * \param uri the uri we want to get with this request.
     * \returns a promise which receives the http response message. It is a good idea to parse it using the helper
     * methods provided by this class.
     */
    IAsyncOperationWithProgress<HttpResponseMessage^, HttpProgress>^ Get(String^ uri);

    /**
     * \brief This method provides a helper method for parsing message and fetching the response payload as string.
     *
     * \param msg The message we want to parse.
     */
    IAsyncOperation<HttpResponseMessageParsed^>^ ParseResponseWithStringContent(HttpResponseMessage^ msg);
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