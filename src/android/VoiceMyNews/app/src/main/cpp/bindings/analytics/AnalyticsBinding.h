#ifndef VOICEMYNEWSANDROID_BINDINGS_ANALYTICS_ANALYTICSBINDING_H_
#define VOICEMYNEWSANDROID_BINDINGS_ANALYTICS_ANALYTICSBINDING_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Provides the logic for loading the webapp responsible for analytics.
 */
JNIEXPORT jstring JNICALL Java_com_voicemynews_core_bindings_analytics_Analytics_getAnalyticsApp(
    JNIEnv *env,
    jobject objInst);

/**
 * \brief Provides the logic for initializing required native objects for analytics.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_analytics_Analytics_initAnalyticsNative(
    JNIEnv *env,
    jobject objInst);

#ifdef __cplusplus
};
#endif

#endif /* VOICEMYNEWSANDROID_BINDINGS_ANALYTICS_ANALYTICSBINDING_H_ */