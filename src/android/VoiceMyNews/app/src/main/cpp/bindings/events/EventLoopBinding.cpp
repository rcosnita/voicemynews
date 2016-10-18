#include "EventLoopBinding.h"
#include "events/EventLoop.h"

#include <exception>

static jclass EmitterCls = nullptr;
static jmethodID EmitterConstructorId = nullptr;

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_emit(
    JNIEnv* env,
    jobject thisObj,
    jstring evtName,
    jobject evtData)
{
    throw std::exception();
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_on(
    JNIEnv* env,
    jobject thisObj,
    jstring evtName,
    jobject evtHandler)
{
    throw std::exception();
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_off(
    JNIEnv* env,
    jobject thisObj,
    jstring listenerId)
{
    throw std::exception();
}

JNIEXPORT jobject JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_getInstanceNative(
    JNIEnv *env,
    jclass objCls)
{
    if (EmitterCls == nullptr)
    {
        EmitterCls = static_cast<jclass>(env->NewGlobalRef(objCls));
        EmitterConstructorId = env->GetMethodID(EmitterCls, "<init>", "(J)V");
    }

    auto eventLoop = new voicemynews::core::events::EventLoop();
    jlong eventLoopPtr = reinterpret_cast<uintptr_t>(eventLoop);
    auto javaLoop = env->NewObject(EmitterCls, EmitterConstructorId, eventLoopPtr);

    return javaLoop;
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_destroyNative(
    JNIEnv* env,
    jclass objCls,
    jlong emitterPtr)
{
    auto emitterPtrId = static_cast<uintptr_t>(emitterPtr);
    auto emitter = reinterpret_cast<voicemynews::core::events::EventLoop*>(emitterPtrId);

    delete emitter;
}