#include "Analytics.h"
#include "config/Config.h"
#include "events/EventNames.h"

#include <sstream>

using voicemynews::core::events::kAnalyticsLogEvent;

namespace voicemynews {
namespace core {
namespace analytics {
AnalyticsEvent::AnalyticsEvent(std::string eventCategory, std::string eventAction,
                               std::string eventLabel, int eventValue)
    : eventCategory_(eventCategory),
      eventAction_(eventAction),
      eventLabel_(eventLabel),
      eventValue_(eventValue)
{
}

std::string AnalyticsEvent::EventCategory() const
{
    return eventCategory_;
}

std::string AnalyticsEvent::EventAction() const
{
    return eventAction_;
}

std::string AnalyticsEvent::EventLabel() const
{
    return eventLabel_;
}

int AnalyticsEvent::EventValue() const
{
    return eventValue_;
}

AnalyticsEventTypeEnum AnalyticsEvent::EventType() const
{
    return AnalyticsEventTypeEnum::CustomEvent;
}

std::string AnalyticsEvent::ToJson() const
{
    std::ostringstream json;
    json << "{\"eventCategory\": \"" << EventCategory() << "\",";
    json << "\"eventAction\": \"" << EventAction() << "\",";
    json << "\"eventLabel\": \"" << EventLabel() << "\",";
    json << "\"eventValue\": " << EventValue() << ",";
    json << "\"custom-eventType\": " << static_cast<int>(EventType()) << ",";
    json << "\"custom-eventName\": \"" << kAnalyticsLogEvent << "\"";
    json << "}";

    return json.str();
}

Analytics::Analytics(std::shared_ptr<WebBrowser> webBrowser)
    : webBrowser_(webBrowser)
{
}

AnalyticsScreenEvent::AnalyticsScreenEvent(std::string screenName)
    : AnalyticsEvent(screenName, "", "", 0)
{
}

AnalyticsEventTypeEnum AnalyticsScreenEvent::EventType() const
{
    return AnalyticsEventTypeEnum::ScreenEvent;
}

std::string Analytics::LoadAnalyticsHtmlApp(voicemynews::core::io::fs::FileUtils& fileUtils)
{
    auto html = fileUtils.ReadFilePlatform("js/html/analytics/app.html");
    auto js = fileUtils.ReadFilePlatform("js/html/analytics/analytics.js");
    std::string htmlStd(html.begin(), html.end());
    std::string jsStd(js.begin(), js.end());
    auto analyticsAppLoc = htmlStd.find("{{analytics_app_js}}");
    htmlStd = htmlStd.replace(analyticsAppLoc, std::strlen("{{analytics_app_js}}"), jsStd);

    auto trackingJs = voicemynews::core::config::kAnalyticsGaTrackingCode;
    auto analyticsTrackingLoc = htmlStd.find("{{analytics_tracking_js}}");
    htmlStd = htmlStd.replace(analyticsTrackingLoc, std::strlen("{{analytics_tracking_js}}"), trackingJs);

    return htmlStd;
}

void Analytics::LogEvent(const AnalyticsEvent& evt)
{
    webBrowser_->SendEvent(kAnalyticsLogEvent, evt.ToJson());
}
}
}
}