#include <string>
#include "EventLoopBinding.h"
#include "EventLoopPlatform.h"

#include <string>

using voicemynews::core::events::EventLoop;
using voicemynews::core::events::EventLoopPlatform;

static JavaVM* CurrVM = nullptr;
static jclass EmitterCls = nullptr;
static jmethodID EmitterConstructorId = nullptr;
static jfieldID  EmitterPtrFieldId = nullptr;

/**
 * \brief Provides the logic for obtaining the native event loop from a java event loop binding class.
 */
static EventLoopPlatform* GetEventLoopFromObject(JNIEnv* env, jobject loop)
{
    auto emitterPtr = env->GetLongField(loop, EmitterPtrFieldId);
    auto emitter = reinterpret_cast<EventLoopPlatform*>(emitterPtr);
    return emitter;
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_emit(
    JNIEnv* env,
    jobject thisObj,
    jstring evtName,
    jobject evtData)
{
    auto eventLoop = GetEventLoopFromObject(env, thisObj);
    eventLoop->Emit(env, evtName, evtData);
}

JNIEXPORT jstring JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_on(
    JNIEnv* env,
    jobject thisObj,
    jstring evtName,
    jobject evtHandler)
{
    auto eventLoop = GetEventLoopFromObject(env, thisObj);
    return eventLoop->On(env, evtName, evtHandler);
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_off(
    JNIEnv* env,
    jobject thisObj,
    jstring listenerId)
{
    auto eventLoop = GetEventLoopFromObject(env, thisObj);
    std::string handlerKey = env->GetStringUTFChars(listenerId, nullptr);
    eventLoop->Off(handlerKey);
}

JNIEXPORT jobject JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_getInstanceNative(
    JNIEnv *env,
    jclass objCls)
{
    if (CurrVM == nullptr) {
        env->GetJavaVM(&CurrVM);
    }

    if (EmitterCls == nullptr) {
        EmitterCls = static_cast<jclass>(env->NewGlobalRef(objCls));
        EmitterConstructorId = env->GetMethodID(EmitterCls, "<init>", "(J)V");
        EmitterPtrFieldId = env->GetFieldID(EmitterCls, "nativeEmitterPtr", "J");
    }

    auto eventLoop = EventLoopPlatform::GetInstance();
    jlong eventLoopPtr = reinterpret_cast<uintptr_t>(eventLoop);
    jobject javaLoop = env->NewObject(objCls, EmitterConstructorId, eventLoopPtr);

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