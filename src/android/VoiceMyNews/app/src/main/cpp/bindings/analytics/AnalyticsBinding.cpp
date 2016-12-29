#include "analytics/Analytics.h"
#include "io/fs/FileUtilsPlatform.h"
#include "AnalyticsBinding.h"

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
    auto htmlStd = Analytics::LoadAnalyticsHtmlApp(fileUtils);
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