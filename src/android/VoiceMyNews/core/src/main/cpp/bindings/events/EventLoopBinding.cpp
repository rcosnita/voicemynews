#include "EventLoopBinding.h"
#include "events/EventLoop.h"

#include <exception>

JNICALL void JNIEXPORT Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_emit(
    JNIEnv* env,
    jobject thisObj)
{
    throw std::exception();
}

JNICALL void JNIEXPORT Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_on(
    JNIEnv* env,
    jobject thisObj,
    jstring evtName,
    jobject evtHandler)
{
    throw std::exception();
}

JNICALL void JNIEXPORT Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_off(
    JNIEnv* env,
    jobject thisObj,
    jstring listenerId)
{
    throw std::exception();
}

JNICALL jobject JNIEXPORT Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_getInstanceNative(
    JNIEnv* env,
    jobject thisObj)
{
    throw std::exception();
}