#ifndef VOICEMYNEWSCORE_ANALYTICS_ANALYTICS_H_
#define VOICEMYNEWSCORE_ANALYTICS_ANALYTICS_H_

#include "WebBrowser.h"
#include "io/fs/FileUtils.h"

#include <string>

namespace voicemynews {
namespace core {
namespace analytics {
/**
 * \brief Provides the model for an analytics event. It is compliant with the analytics-layer spec.
 */
class AnalyticsEvent
{
public:
    AnalyticsEvent(std::string eventCategory, std::string eventAction, std::string eventLabel,
        int eventValue);

    std::string EventCategory() const;

    std::string EventAction() const;

    std::string EventLabel() const;

    int EventValue() const;

    /**
     * \brief Builds a json string out of the current event.
     */
    std::string ToJson() const;

private:
    std::string eventCategory_;
    std::string eventAction_;
    std::string eventLabel_;
    int eventValue_;
};

/**
 * \brief Provides the contract for analytics layer.
 *
 * Analytics layer allows voice my news to publish events to a backend analytics solution. The backend analytics
 * solution allows us to understand the behavior of our customers and to take accurate decisions.
 */
class Analytics
{
public:
    Analytics(std::shared_ptr<WebBrowser> webBrowser);

    /**
     * \brief Provides the logic for logging the current analytics event.
     *
     * It simply builds a string from the analytics event and sends it to the web browser.
     */
    void LogEvent(const AnalyticsEvent& evt);

    /**
     * \brief Provides the logic for retrieving the analytics html application.
     *
     * On each platform, this application must be embedded into a web browser or headless dom.
     */
    static std::string LoadAnalyticsHtmlApp(voicemynews::core::io::fs::FileUtils& fileUtils);

private:
    std::shared_ptr<WebBrowser> webBrowser_;
};
}
}
}

#endif // VOICEMYNEWSCORE_ANALYTICS_ANALYTICS_H_