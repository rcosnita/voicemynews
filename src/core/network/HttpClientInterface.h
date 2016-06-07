#ifndef VoiceMyNewsCore_core_network_HttpClientInterface_H_
#define VoiceMyNewsCore_core_network_HttpClientInterface_H_

#include <string>
#include <map>

namespace voicemynews {
namespace core {
namespace network {
/**
 * This pointer function defines the compatible callbacks which can be passed to the http client methods.
 */
typedef void(*HttpClientResponseCallback)(const std::map<std::string, std::string>& headers, const int statusCode, void* data);

/**
 * \class HttpClientInterface
 * \brief This class provides the http contract which must be implemented by each platform.
 *
 * Each platform must be capable to make http requests from javascript and probably even from code.
 * This contract is the key for supporting http operations. Each concrete implementation will typedef
 * to HttpClient and HttpClientNative.
 *
 * HttpClient is the type that can be safely binded to javascript.
 * HttpClientNative is the type that can be safely used in native side of the code.
 */
class HttpClientInterface {
public:
    /**
     * \brief This method provides the contract for being able to do get requests.
     *
     * \param url The url which we want to invoke.
     * \param headers The headers we want to pass with the current request.
     * \param queryParams The query parameters we want to pass with the current request.
     * \param handleResponse The callback used to handle the response of the get operation.
     */
    virtual void Get(const std::string& url, const std::map<std::string, std::string>& headers,
        const std::map<std::string, std::string>& queryParams, HttpClientResponseCallback handleResponse = nullptr) = 0;
};
}
}
}

#endif // VoiceMyNewsCore_core_network_HttpClientInterface_H_