#include "pch.h"

#include <ppltasks.h>

#include "bindings/network/HttpClientBinding.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
using Windows::Foundation::Uri;
using Windows::Web::Http::HttpClient;
using Windows::Web::Http::HttpMethod;

HttpResponseMessageParsed::HttpResponseMessageParsed(int statusCode, String^ reason, HttpContentHeaderCollection^ headers,
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

HttpContentHeaderCollection^ HttpResponseMessageParsed::GetHeaders() {
    return headers_;
}

IAsyncOperationWithProgress<HttpResponseMessage^, HttpProgress>^ HttpClientBinding::Get(Platform::String^ uri,
    IMap<String^, String^>^ requestHeaders) {
    auto httpClient = ref new HttpClient();
    auto requestMessage = ref new HttpRequestMessage(HttpMethod::Get, ref new Uri(uri));

    if (requestHeaders != nullptr) {
        CopyHeadersToRequestMessage(*requestHeaders, *requestMessage);
    }

    return httpClient->SendRequestAsync(requestMessage);
}

IAsyncOperation<HttpResponseMessageParsed^>^ HttpClientBinding::ParseResponseWithStringContent(HttpResponseMessage^ msg) {
    return concurrency::create_async([msg]() {
        String^ content = concurrency::create_task(msg->Content->ReadAsStringAsync()).get();

        return ref new HttpResponseMessageParsed(static_cast<int>(msg->StatusCode), msg->ReasonPhrase, msg->Content->Headers,
            content);
    });
}

void HttpClientBinding::CopyHeadersToRequestMessage(IMap<String^, String^>% headers,
    HttpRequestMessage% requestMessage) {
    for (auto entry = headers.First(); entry->HasCurrent; entry->MoveNext()) {
        requestMessage.Headers->Append(entry->Current->Key, entry->Current->Value);
    }
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