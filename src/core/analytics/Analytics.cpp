#include "Analytics.h"
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

std::string AnalyticsEvent::ToJson() const
{
    std::ostringstream json;
    json << "{\"eventCategory\": \"" << EventCategory() << "\",";
    json << "\"eventAction\": \"" << EventAction() << "\",";
    json << "\"eventLabel\": \"" << EventLabel() << "\",";
    json << "\"eventValue\": " << EventValue() << ",";
    json << "\"custom-eventName\": \"" << kAnalyticsLogEvent << "\"";
    json << "}";

    return json.str();
}

Analytics::Analytics(std::shared_ptr<WebBrowser> webBrowser)
    : webBrowser_(webBrowser)
{
}

void Analytics::LogEvent(const AnalyticsEvent& evt)
{
    webBrowser_->SendEvent(kAnalyticsLogEvent, evt.ToJson());
}
}
}
}