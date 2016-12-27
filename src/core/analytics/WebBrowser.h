#ifndef VOICEMYNEWSCORE_ANALYTICS_WEBBROWSER_H_
#define VOICEMYNEWSCORE_ANALYTICS_WEBBROWSER_H_

#include <memory>
#include <string>

namespace voicemynews {
namespace core {
namespace analytics {
/**
 * \brief Provides a web browser abstraction. A web browser is required in order to load analytics engines.
 *
 * Each platform can implement this contract as it sees fit. It is a must that each implementation enables JS support
 * in the web browser.
 */
class WebBrowser
{
private:
    WebBrowser() = default;

public:
    /**
     * \brief Allows developers to load valid html content into the web browser.
     */
    virtual void LoadContent(std::string webPageContent) = 0;

    /**
     * \brief Allows developers to send messages to the web browser.
     *
     * Depending on the platform, this might be implemented using post messages or projections.
     */
    virtual void SendEvent(std::string evtName, std::string evtData) = 0;

    /**
     * \brief Obtains an instance of the web browser.
     */
    virtual std::shared_ptr<WebBrowser> GetInstance() = 0;
};
}
}
}

#endif /* VOICEMYNEWSCORE_ANALYTICS_WEBBROWSER_H_ */