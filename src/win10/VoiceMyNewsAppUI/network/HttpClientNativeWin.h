#ifndef VoiceMyNewsAppUI_win10_network_HttpClientNativeWin_H_
#define VoiceMyNewsAppUI_win10_network_HttpClientNativeWin_H_

#include "network/HttpClientInterface.h"

namespace voicemynews {
namespace core {
namespace network {
/**
* \class HttpClientNativeWin
* \brief This class provides the native http client which can be used by native code.
*/
template<typename T>
class HttpClientNativeWin : public HttpClientInterface {
public:
    HttpClientNativeWin(T^ httpClient)
        : httpClient_(httpClient) {
    }

    virtual void Get(const std::string& url, const std::map<std::string, std::string>& headers,
        const std::map<std::string, std::string>& queryParams, HttpClientResponseCallback handleResponse = nullptr);

private:
    T^ httpClient_;
};
}
}
}

#endif // VoiceMyNewsAppUI_win10_network_HttpClientNativeWin_H_