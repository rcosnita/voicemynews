#ifndef VOICEMYNEWS_EVENTEMITTERJNI_H
#define VOICEMYNEWS_EVENTEMITTERJNI_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Provides the implementation for EventLoopBindingNative emit method.
 *
 * Internally, it relies on the native event emitter implementation. See EventLoop.h for more details.
 */
JNICALL void JNIEXPORT Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_emit(
    JNIEnv* env,
    jobject thisObj);

/**
 * \brief Provides the implementation for EventLoopBindingNative on method.
 *
 * Internally, it relies on the native event emitter implementation. See EventLoop.h for mor details.
 */
JNICALL void JNIEXPORT Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_on(
    JNIEnv* env,
    jobject thisObj,
    jstring evtName,
    jobject evtHandler);

/**
 * \brief Provides the implementation for EventLoopBindingNative off method.
 *
 * Internally, it relies on the native event emitter implementation. See EventLoop.h for mor details.
 */
JNICALL void JNIEXPORT Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_off(
    JNIEnv* env,
    jobject thisObj,
    jstring listenerId);

/**
 * \brief Provides the implementation for EventLoopBindingNative getInstanceNative method.
 *
 * Internally, it relies on the native event emitter implementation. See EventLoop.h for mor details.
 */
JNICALL jobject JNIEXPORT Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_getInstanceNative(
    JNIEnv* env,
    jobject thisObj);

#ifdef __cplusplus
}
#endif

#endif //VOICEMYNEWS_EVENTEMITTERJNI_H
