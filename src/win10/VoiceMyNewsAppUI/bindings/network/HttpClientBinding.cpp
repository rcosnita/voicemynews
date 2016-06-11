#include "pch.h"

#include <ppltasks.h>

#include "bindings/network/HttpClientBinding.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
using Windows::Foundation::Uri;
using Windows::Web::Http::HttpClient;

HttpResponseMessageParsed::HttpResponseMessageParsed(int statusCode, String^ reason, HttpResponseHeaderCollection^ headers,
    String^ content)
    : statusCode_(statusCode),
      reason_(reason),
      headers_(headers),
      content_(content) {
}

int HttpResponseMessageParsed::GetStatusCode() {
    return statusCode_;
}

String^ HttpResponseMessageParsed::GetReason() {
    return reason_;
}

String^ HttpResponseMessageParsed::GetContent() {
    return content_;
}

HttpResponseHeaderCollection^ HttpResponseMessageParsed::GetHeaders() {
    return headers_;
}

IAsyncOperationWithProgress<HttpResponseMessage^, HttpProgress>^ HttpClientBinding::Get(Platform::String^ uri) {
    auto httpClient = ref new HttpClient();
    return httpClient->GetAsync(ref new Uri(uri));
}

IAsyncOperation<HttpResponseMessageParsed^>^ HttpClientBinding::ParseResponseWithStringContent(HttpResponseMessage^ msg) {
    return concurrency::create_async([msg]() {
        String^ content = concurrency::create_task(msg->Content->ReadAsStringAsync()).get();

        return ref new HttpResponseMessageParsed(static_cast<int>(msg->StatusCode), msg->ReasonPhrase, msg->Headers,
            content);
    });
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace network {
typedef voicemynews::app::win10::bindings::HttpResponseMessageParsed HttpResponseMessageParsed;

HttpClientBinding^ HttpClient::GetInstance() {
    return ref new HttpClientBinding();
}
}
}
}