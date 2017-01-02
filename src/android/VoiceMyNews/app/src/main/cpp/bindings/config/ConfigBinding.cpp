#include "ConfigBinding.h"
#include "config/Config.h"

#include <string>

static void SetClassMember(JNIEnv*& env, jclass objCls, std::string attrName, std::string attrValue)
{
    auto fieldId = env->GetStaticFieldID(objCls, attrName.c_str(), "Ljava/lang/String;");
    env->SetStaticObjectField(objCls, fieldId, env->NewStringUTF(attrValue.c_str()));
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_config_Config_initConfig(
    JNIEnv* env,
    jclass objCls)
{
    SetClassMember(env, objCls, "WEB_BASE_URL", voicemynews::core::config::kWebBaseUrl);
    SetClassMember(env, objCls, "ANALYTICS_GA_TRACKING_CODE", voicemynews::core::config::kAnalyticsGaTrackingCode);
}