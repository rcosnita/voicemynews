#include "pch.h"
#include <ppltasks.h>

#include "bindings/network/HttpClientBinding.h"

using Windows::Foundation::Uri;
using Windows::Web::Http::HttpClient;
using Windows::Web::Http::HttpMethod;
using Platform::Collections::Map;
using voicemynews::app::win10::js::JsApp;
using voicemynews::app::win10::bindings::events::JsLoopEnqueuedTask;

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
HttpResponseMessageParsed::HttpResponseMessageParsed(int statusCode, String^ reason, HttpContentHeaderCollection^ headers,
    String^ content)
    : statusCode_(statusCode),
      reason_(reason),
      headers_(headers),
      content_(content)
{
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

HttpClientBinding::HttpClientBinding()
{
    jsLoop_ = JsApp::GetInstance()->GetEventLoop();
}

IAsyncOperationWithProgress<HttpResponseMessage^, HttpProgress>^ HttpClientBinding::Get(Platform::String^ uri,
    HttpClientBinding::HeadersStorage^ requestHeaders) {
    auto httpClient = ref new HttpClient();
    auto requestMessage = ref new HttpRequestMessage(HttpMethod::Get, ref new Uri(uri));

    if (requestHeaders != nullptr) {
        CopyHeadersToRequestMessage(*requestHeaders, *requestMessage);
    }

    return httpClient->SendRequestAsync(requestMessage);
}

void HttpClientBinding::Get(String^ uri, HttpClientBinding::HeadersStorage^ requestHeaders, HttpClientBindingRequestOnSuccess^ onSuccess)
{
    concurrency::create_task(Get(uri, requestHeaders))
        .then([this, onSuccess](HttpResponseMessage^ responseMessage) {
        jsLoop_->EnqueueTask(ref new JsLoopEnqueuedTask([onSuccess, responseMessage]() {
            onSuccess(responseMessage);
        }));
    });
}

IAsyncOperation<HttpResponseMessageParsed^>^ HttpClientBinding::ParseResponseWithStringContent(HttpResponseMessage^ msg) {
    return concurrency::create_async([msg]() {
        String^ content = concurrency::create_task(msg->Content->ReadAsStringAsync()).get();

        return ref new HttpResponseMessageParsed(static_cast<int>(msg->StatusCode), msg->ReasonPhrase, msg->Content->Headers,
            content);
    });
}

void HttpClientBinding::ParseResponseWithStringContent(HttpResponseMessage^ msg, HttpClientBindingRequestOnParsed^ onParsed)
{
    concurrency::create_task(ParseResponseWithStringContent(msg))
        .then([this, onParsed](HttpResponseMessageParsed^ messageParsed) {
        jsLoop_->EnqueueTask(ref new JsLoopEnqueuedTask([onParsed, messageParsed] {
            onParsed(messageParsed);
        }));
    });
}

HttpClientBinding::HeadersStorage^ HttpClientBinding::GetNewHeadersMap()
{
    return ref new Map<String^, String^>();
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