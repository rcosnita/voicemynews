#include "pch.h"
#include "AnalyticsBinding.h"
#include "analytics/WebBrowser.h"
#include "io/fs/FileUtils.h"

using voicemynews::core::analytics::Analytics;
using voicemynews::core::analytics::AnalyticsEvent;
using voicemynews::core::analytics::WebBrowser;

static voicemynews::app::win10::bindings::analytics::AnalyticsBinding^ AnalyticsInstance = nullptr;

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace analytics {
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
    analyticsBinding->analytics = std::make_shared<Analytics>(webBrowser);

    voicemynews::core::io::fs::FileUtils fileUtils;
    auto htmlStd = analyticsBinding->analytics->LoadAnalyticsHtmlApp(fileUtils);
    webBrowser->LoadContent(htmlStd);

    AnalyticsEvent evt("test", "click", "play video", 1);
    analyticsBinding->analytics->LogEvent(evt);
}
}
}
}
}
}
