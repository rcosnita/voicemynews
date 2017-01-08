#include "pch.h"
#include "AnalyticsBinding.h"
#include "analytics/AnalyticsConstants.h"
#include "analytics/WebBrowser.h"
#include "events/EventNames.h"
#include "bindings/events/EventLoopBinding.h"
#include "io/fs/FileUtils.h"
#include "utils/Conversions.h"

using voicemynews::core::analytics::Analytics;
using voicemynews::core::analytics::AnalyticsEvent;
using voicemynews::core::analytics::AnalyticsScreenEvent;
using voicemynews::core::analytics::AnalyticsEventTypeEnum;
using voicemynews::core::analytics::WebBrowser;

using voicemynews::app::win10::utils::ConvertPlatformStrToStd;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::core::events::kAnalyticsLayerStarted;

static voicemynews::app::win10::bindings::analytics::AnalyticsBinding^ AnalyticsInstance = nullptr;

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace analytics {
AnalyticsBindingEvent::AnalyticsBindingEvent(int eventType, String^ eventCategory, String^ eventAction,
    String^ eventLabel, int eventValue)
    : eventType_(eventType),
    eventCategory_(eventCategory),
    eventAction_(eventAction),
    eventLabel_(eventLabel),
    eventValue_(eventValue)
{
}

int AnalyticsBindingEvent::EventType()
{
    return eventType_;
}

AnalyticsBindingEvent::String^ AnalyticsBindingEvent::EventCategory()
{
    return eventCategory_;
}

AnalyticsBindingEvent::String^ AnalyticsBindingEvent::EventAction()
{
    return eventAction_;
}

AnalyticsBindingEvent::String^ AnalyticsBindingEvent::EventLabel()
{
    return eventLabel_;
}

int AnalyticsBindingEvent::EventValue()
{
    return eventValue_;
}

AnalyticsBindingEvent::operator voicemynews::core::analytics::AnalyticsEvent()
{
    auto eventCategory = ConvertPlatformStrToStd(EventCategory());
    auto eventAction = ConvertPlatformStrToStd(EventAction());
    auto eventLabel = ConvertPlatformStrToStd(EventLabel());

    if (EventType() == static_cast<int>(AnalyticsEventTypeEnum::CustomEvent)) {
        return AnalyticsEvent(eventCategory, eventAction, eventLabel, EventValue());
    }

    return AnalyticsScreenEvent(eventCategory);
}

AnalyticsBinding::AnalyticsBinding() { }

AnalyticsBinding^ AnalyticsBinding::GetInstance()
{
    if (AnalyticsInstance == nullptr) {
        AnalyticsInstance = ref new AnalyticsBinding();
    }

    return AnalyticsInstance;
}

void AnalyticsBinding::StartAnalytics()
{
    auto analyticsBinding = GetInstance();
    auto webBrowser = WebBrowser::GetInstance();
    analyticsBinding->analytics_ = std::make_shared<Analytics>(webBrowser);

    voicemynews::core::io::fs::FileUtils fileUtils;
    auto htmlStd = analyticsBinding->analytics_->LoadAnalyticsHtmlApp(fileUtils);
    webBrowser->LoadContent(htmlStd);

    auto evt = ref new AnalyticsBindingEvent(
                static_cast<int>(AnalyticsEventTypeEnum::CustomEvent),
                ConvertStdStrToPlatform(voicemynews::core::analytics::constants::categories::kAppLifecycle),
                ConvertStdStrToPlatform(voicemynews::core::analytics::constants::actions::kAppStartAction),
                ConvertStdStrToPlatform(voicemynews::core::analytics::constants::labels::kAppStartLabel), 1);
    analyticsBinding->LogEvent(evt);

    auto evtLoop = voicemynews::app::win10::bindings::events::EventLoopBinding::GetInstance();
    evtLoop->Emit(ConvertStdStrToPlatform(kAnalyticsLayerStarted), ref new EventDataBinding(""));
}

AnalyticsBindingEvent^ AnalyticsBinding::BuildEvent(int evtType, String^ evtCategory, String^ evtAction,
    String^ evtLabel, int evtValue) {
    return ref new AnalyticsBindingEvent(evtType, evtCategory, evtAction, evtLabel, evtValue);
}

void AnalyticsBinding::LogEvent(AnalyticsBindingEvent^ evt)
{
    analytics_->LogEvent(evt);
}
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace analytics {
/**
 * \brief Provides the factory for obtaining instances of analytics objects.
 *
 * This factory is projected in js side and used by the business logic.
 */
public ref class AnalyticsFactory sealed
{
private:
using AnalyticsBinding = voicemynews::app::win10::bindings::analytics::AnalyticsBinding;

public:
    static AnalyticsBinding^ GetInstance()
    {
        return AnalyticsBinding::GetInstance();
    }
};
}
}
}
