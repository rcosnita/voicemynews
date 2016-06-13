#include "pch.h"
#include "network/HttpClientInterface.h"
#include "network/HttpClientNativeWin.h"

namespace voicemynews {
namespace core {
namespace network {
using voicemynews::app::win10::bindings::HttpClientBinding;
using voicemynews::app::win10::bindings::HttpResponseMessageParsed;
using voicemynews::core::network::HttpClient;

std::shared_ptr<HttpClientInterface> HttpClientInterface::GetHttpClientInstance() {
    auto httpClientPlatform = HttpClient::GetInstance();
    std::shared_ptr<HttpClientNativeWin<HttpClientBinding, HttpResponseMessageParsed>> httpClientStd(
        new HttpClientNativeWin<HttpClientBinding, HttpResponseMessageParsed>(httpClientPlatform));

    return httpClientStd;
}
}
}
}