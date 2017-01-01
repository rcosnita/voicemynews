#ifndef VOICEMYNEWSANDROID_BINDINGS_ANALYTICS_ANALYTICSBINDING_H_
#define VOICEMYNEWSANDROID_BINDINGS_ANALYTICS_ANALYTICSBINDING_H_

#include <jni.h>
#include <v8.h>

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace analytics {
    /**
     * \brief Provides the logic for binding the analytics layer to the running js vm.
     *
     * All objects / methods will be binded under the given analyticsNamespace.
     */
    void BindAnalyticsToJs(v8::Isolate* isolate, v8::Persistent<v8::ObjectTemplate>* analyticsNamespace);
}
}
}
}
}

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