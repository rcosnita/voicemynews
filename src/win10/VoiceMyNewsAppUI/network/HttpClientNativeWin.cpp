#include "pch.h"

#include <ppltasks.h>

#include "network/HttpClientNativeWin.h"
#include "utils/Conversions.h"

namespace voicemynews {
namespace core {
namespace network {
using Platform::String;

using voicemynews::app::win10::utils::ConvertPlatformMapToStd;
using Windows::Foundation::Uri;
using Windows::Web::Http::HttpResponseMessage;

template<typename T>
void HttpClientNativeWin<T>::Get(const std::string& url, const std::map<std::string, std::string>& headers,
    const std::map<std::string, std::string>& queryParams, HttpClientResponseCallback handleResponse) {
    String^ urlPlatform = ref new String(std::wstring(url.begin(), url.end()).c_str());
    auto uri = ref new Uri(urlPlatform);

    concurrency::create_task(httpClient_->GetAsync(uri)).then([handleResponse](HttpResponseMessage^ response) {
        auto responseHeaders = ConvertPlatformMapToStd(response->Headers);

        handleResponse(responseHeaders, (int)response->StatusCode, (void*)response->Content);
    });
}
}
}
}