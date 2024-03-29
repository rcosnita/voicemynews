#ifndef VoiceMyNewsAppUI_win10_network_HttpClientNativeWin_H_
#define VoiceMyNewsAppUI_win10_network_HttpClientNativeWin_H_

#include "network/HttpClientInterface.h"
#include "utils/Conversions.h"

#include <collection.h>
#include <ppltasks.h>
#include <string>

namespace voicemynews {
namespace core {
namespace network {
using Platform::Collections::Map;
using Platform::Exception;
using Platform::String;

using voicemynews::app::win10::utils::ConvertPlatformMapToStd;
using voicemynews::app::win10::utils::ConvertPlatformStrToStd;
using voicemynews::app::win10::utils::ConvertStdMapToPlatform;

using Windows::Foundation::Uri;
using Windows::Web::Http::Headers::HttpContentHeaderCollection;
using Windows::Web::Http::HttpResponseMessage;
/**
 * \class HttpClientNativeWin
 * \brief This class provides the native http client which can be used by native code.
 */
template<typename TClientType, typename TParsedResponseTypeStr>
class HttpClientNativeWin : public HttpClientInterface {
public:
    HttpClientNativeWin(TClientType^ httpClient)
        : httpClient_(httpClient) {
    }

    /**
     * \brief This method provides the windows implementation for get method which returns a response with string content.
     */
    virtual void Get(const std::string& url, const std::map<std::string, std::string>& headers,
        const std::map<std::string, std::string>& queryParams, HttpClientResponseStringCallback handleResponse = nullptr) {
        String^ uriPlatform = ref new String(std::wstring(url.begin(), url.end()).c_str());

        auto requestHeaders = ConvertStdMapToPlatform(headers);
        auto taskGetUri = concurrency::create_task(httpClient_->Get(uriPlatform, requestHeaders));

        try {
            taskGetUri.then([this, handleResponse](HttpResponseMessage^ response) {
                concurrency::create_task(httpClient_->ParseResponseWithStringContent(response))
                    .then([handleResponse, &response](TParsedResponseTypeStr^ responseParsed) {
                    int statusCode = responseParsed->GetStatusCode();
                    auto headers = ConvertPlatformMapToStd(responseParsed->GetHeaders());
                    auto reason = ConvertPlatformStrToStd(responseParsed->GetReason());
                    auto content = ConvertPlatformStrToStd(responseParsed->GetContent());

                    auto responseStd = std::shared_ptr<HttpResponseData<std::string>>(new HttpResponseData<std::string>(statusCode, headers, reason, content));

                    handleResponse(responseStd);
                });
            }).wait();
        }
        catch (Exception^ ex) {
            // TODO [rcosnita] at this point we assume the uri was not found. We shall interpret HRESULT code correctly.
            auto responseStd = std::shared_ptr<HttpResponseData<std::string>>(
                new HttpResponseData<std::string>(404, std::map<std::string, std::string>(), "Not Found", std::string()));

            handleResponse(responseStd);
        }
    }

private:
    TClientType^ httpClient_;
};
}
}
}

#endif // VoiceMyNewsAppUI_win10_network_HttpClientNativeWin_H_