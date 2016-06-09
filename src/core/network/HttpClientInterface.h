#ifndef VoiceMyNewsCore_core_network_HttpClientInterface_H_
#define VoiceMyNewsCore_core_network_HttpClientInterface_H_

#include <string>
#include <map>
#include <memory>

namespace voicemynews {
namespace core {
namespace network {
/**
 * \class HttpResponseData
 * \brief This class provides a cross platform representation of a http response.
 */
template<typename T>
class HttpResponseData {
public:
    HttpResponseData(int statusCode, std::map<std::string, std::string> headers, std::string reason, T content)
        : statusCode_(statusCode),
          headers_(headers),
          reason_(reason),
          content_(content) {
    }

    /**
     * This method returns the status code from the response.
     */
    int GetStatusCode() const { return statusCode_; }

    /**
     * This method obtains the reason phrase from the response.
     */
    std::string GetReason() const { return reason_; }

    /**
     * This method obtains the content from the http response.
     */
    std::string GetContent() const { return content_; }

    /**
     * This method obtains the response headers.
     */
    std::map<std::string, std::string> GetHeaders() const { return headers_; }
private:
    int statusCode_;
    std::string reason_;
    std::string content_;
    std::map<std::string, std::string> headers_;
};

/**
 * This pointer function defines the compatible callbacks which can be passed to the http client methods.
 */
typedef void(*HttpClientResponseStringCallback)(std::shared_ptr<HttpResponseData<std::string>>);

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
        const std::map<std::string, std::string>& queryParams, HttpClientResponseStringCallback handleResponse = nullptr) = 0;
};
}
}
}

#endif // VoiceMyNewsCore_core_network_HttpClientInterface_H_