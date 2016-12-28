#include "analytics/Analytics.h"
#include "AnalyticsBinding.h"

#include "io/fs/FileUtilsPlatform.h"
#include <exception>

using voicemynews::core::io::fs::FileUtilsPlatform;
using voicemynews::core::analytics::Analytics;
using voicemynews::core::analytics::AnalyticsEvent;
using voicemynews::core::analytics::WebBrowser;

static Analytics* AnalyticsInstance = nullptr;

JNIEXPORT jstring JNICALL Java_com_voicemynews_core_bindings_analytics_Analytics_getAnalyticsApp(
    JNIEnv* env,
    jobject objInst)
{
    FileUtilsPlatform fileUtils;
    auto html = fileUtils.ReadFilePlatform("js/html/analytics/app.html");
    auto js = fileUtils.ReadFilePlatform("js/html/analytics/analytics.js");
    std::string htmlStd(html.begin(), html.end());
    std::string jsStd(js.begin(), js.end());
    auto analyticsAppLoc = htmlStd.find("{{analytics_app_js}}");
    htmlStd = htmlStd.replace(analyticsAppLoc, std::strlen("{{analytics_app_js}}"), jsStd);

    return env->NewStringUTF(htmlStd.c_str());
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_analytics_Analytics_initAnalyticsNative(
    JNIEnv *env,
    jobject objInst)
{
    AnalyticsInstance = new Analytics(WebBrowser::GetInstance());

    AnalyticsEvent evt("test", "click", "play video", 1);
    AnalyticsInstance->LogEvent(evt);
}