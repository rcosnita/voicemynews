#ifndef VOICEMYNEWSANDROID_BINDINGS_CONFIG_CONFIGBINDING_H_
#define VOICEMYNEWSANDROID_BINDINGS_CONFIG_CONFIGBINDING_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_config_Config_initConfig(
    JNIEnv* env,
    jclass objCls);

#ifdef __cplusplus
};
#endif

#endif /* VOICEMYNEWSANDROID_BINDINGS_CONFIG_CONFIGBINDING_H_ */