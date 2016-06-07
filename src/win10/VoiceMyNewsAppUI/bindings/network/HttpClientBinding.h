#ifndef VoiceMyNewsAppUI_win10_bindings_network_HttpClientBinding_H_
#define VoiceMyNewsAppUI_win10_bindings_network_HttpClientBinding_H_

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
using Windows::Foundation::IAsyncOperationWithProgress;
using Windows::Foundation::Uri;
using Windows::Web::Http::HttpProgress;
using Windows::Web::Http::HttpResponseMessage;

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
     */
    IAsyncOperationWithProgress<HttpResponseMessage^, HttpProgress>^ Get(Uri^ uri);
};
}
}
}
}

namespace voicemynews {
namespace core {
namespace network {
typedef voicemynews::app::win10::bindings::HttpClientBinding HttpClientPlatform;

/**
 * \brief This class provides the factory implementation for obtaining a platform specific instance of HttpClient.
 */
public ref class HttpClient sealed {
public:
    static HttpClientPlatform^ GetInstance();
};
}
}
}

#endif // VoiceMyNewsCore_win10_bindings_network_HttpClientBinding_H_