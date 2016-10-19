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
 *
 * \param evtName the event name we want to emit.
 * \param evtData the event data we want to pass to all listeners. This has com.voicemynews.core.bindings.events.EventDataBinding type.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_emit(
    JNIEnv* env,
    jobject thisObj,
    jstring evtName,
    jobject evtData);

/**
 * \brief Provides the implementation for EventLoopBindingNative on method.
 *
 * Internally, it relies on the native event emitter implementation. See EventLoop.h for mor details.
 */
JNIEXPORT jstring JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_on(
    JNIEnv* env,
    jobject thisObj,
    jstring evtName,
    jobject evtHandler);

/**
 * \brief Provides the implementation for EventLoopBindingNative off method.
 *
 * Internally, it relies on the native event emitter implementation. See EventLoop.h for mor details.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_off(
    JNIEnv* env,
    jobject thisObj,
    jstring listenerId);

/**
 * \brief Provides the implementation for EventLoopBindingNative getInstanceNative method.
 *
 * Internally, it relies on the native event emitter implementation. See EventLoop.h for mor details.
 */
JNIEXPORT jobject JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_getInstanceNative(
    JNIEnv *env,
    jclass objCls);

/**
 * \brief Provides the implementation for EventLoopBindingNative destroyNative method.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_destroyNative(
    JNIEnv* env,
    jclass objCls,
    jlong emitterPtr);

#ifdef __cplusplus
}
#endif

#endif //VOICEMYNEWS_EVENTEMITTERJNI_H
