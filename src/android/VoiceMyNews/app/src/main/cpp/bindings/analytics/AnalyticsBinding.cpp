#include "AnalyticsBinding.h"
#include "analytics/AnalyticsConstants.h"
#include "analytics/Analytics.h"
#include "bindings/events/EventLoopBinding.h"
#include "bindings/events/EventLoopPlatform.h"
#include "io/fs/FileUtilsPlatform.h"
#include "utils/DataWrapper.h"

#include <memory>
#include <events/EventNames.h>

using voicemynews::app::android::utils::DataWrapper;
using voicemynews::core::io::fs::FileUtilsPlatform;
using voicemynews::core::analytics::Analytics;
using voicemynews::core::analytics::AnalyticsEvent;
using voicemynews::core::analytics::AnalyticsEventTypeEnum;
using voicemynews::core::analytics::AnalyticsScreenEvent;
using voicemynews::core::analytics::WebBrowser;

static Analytics* AnalyticsInstance = nullptr;

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace analytics {
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Local;
using v8::External;
using v8::Isolate;
using v8::ObjectTemplate;
using v8::Persistent;
using v8::String;
using v8::Value;

/**
 * \brief Log the received first argument as an analytics event. It uses the currently available AnalyticsInstance.
 */
static void LogAnalyticsNativeEvent(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input parameters.
    auto evtNativePtr = Local<External>::Cast(info[0]->ToObject()->GetInternalField(0)->ToObject())->Value();
    auto evtNative = reinterpret_cast<DataWrapper<std::shared_ptr<AnalyticsEvent>>*>(evtNativePtr);

    AnalyticsInstance->LogEvent(*evtNative->Data());

    delete evtNative;
}

/**
 * \brief Provides the js wrapper for building a native log event on js side.
 */
static void BuildAnalyticsNativeEvent(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input parameters.
    auto isolate = info.GetIsolate();

    auto evtType = info[0]->ToNumber()->Int32Value();
    auto evtCategory = std::string(*String::Utf8Value(info[1]->ToString()));
    auto evtAction = std::string(*String::Utf8Value(info[2]->ToString()));
    auto evtLabel = std::string(*String::Utf8Value(info[3]->ToString()));
    auto evtValue = info[4]->ToNumber()->Int32Value();

    auto evt = ObjectTemplate::New(isolate);
    evt->SetInternalFieldCount(1);

    auto evtInst = evt->NewInstance();
    AnalyticsEvent* evtInstance = nullptr;

    if (evtType == static_cast<int>(AnalyticsEventTypeEnum::CustomEvent))
    {
        evtInstance = new AnalyticsEvent(evtCategory, evtAction, evtLabel, evtValue);
    }
    else
    {
        evtInstance = new AnalyticsScreenEvent(evtCategory);
    }

    auto evtNative = new DataWrapper<std::shared_ptr<AnalyticsEvent>>(std::shared_ptr<AnalyticsEvent>(evtInstance));
    evtInst->SetInternalField(0, External::New(isolate, evtNative));

    info.GetReturnValue().Set(evtInst);
}

/**
 * \brief Provides the implementation for obtaining an analytics instance object which can be used in js.
 */
static void GetAnalyticsInstance(const FunctionCallbackInfo<Value>& info)
{
    auto isolate = info.GetIsolate();
    auto analyticsObj = ObjectTemplate::New(info.GetIsolate());

    analyticsObj->Set(isolate, "buildEvent", FunctionTemplate::New(isolate, BuildAnalyticsNativeEvent));
    analyticsObj->Set(isolate, "logEvent", FunctionTemplate::New(isolate, LogAnalyticsNativeEvent));

    auto objInst = analyticsObj->NewInstance();
    info.GetReturnValue().Set(objInst);
}

void BindAnalyticsToJs(v8::Isolate* isolate, v8::Persistent<v8::ObjectTemplate>* analyticsNamespace)
{
    auto voicemynewsAnalytics = analyticsNamespace->Get(isolate);
    auto analyticsFactory = ObjectTemplate::New();

    voicemynewsAnalytics->Set(isolate, "AnalyticsFactory", analyticsFactory);
    analyticsFactory->Set(isolate, "getInstance", FunctionTemplate::New(isolate, GetAnalyticsInstance));
}
}
}
}
}
}

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

    AnalyticsEvent evt(
        voicemynews::core::analytics::constants::categories::kAppLifecycle,
        voicemynews::core::analytics::constants::actions::kAppStartAction,
        voicemynews::core::analytics::constants::labels::kAppStartLabel, 1);
    AnalyticsInstance->LogEvent(evt);

    auto evtLoop = voicemynews::core::events::EventLoopPlatform::GetInstance();
    auto evtData = std::shared_ptr<voicemynews::core::events::EventData<std::string>>(new voicemynews::app::android::bindings::events::EventDataBinding(""));
    static_cast<voicemynews::core::events::EventLoop*>(evtLoop)->Emit(voicemynews::core::events::kAnalyticsLayerStarted, evtData);
}
