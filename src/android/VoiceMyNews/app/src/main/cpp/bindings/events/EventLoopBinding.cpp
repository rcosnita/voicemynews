#include <string>
#include "EventLoopBinding.h"
#include "events/EventLoop.h"
#include "utils/StringHelpers.h"

#include <exception>

using voicemynews::app::android::utils::StringHelpers;

static JavaVM* CurrVM = nullptr;
static jclass EmitterCls = nullptr;
static jmethodID EmitterConstructorId = nullptr;

/**
 * \brief This method constructs a unique string identifier for the given event handler pointer.
 */
static std::string GetStrForEventHandlerPtr(JNIEnv *env,
    std::string evtName,
    jclass handlerClass,
    jobject evtHandler)
{
    jmethodID getClassId = env->GetMethodID(handlerClass, "getClass", "()Ljava/lang/Class;");
    jobject handlerClassRefl = env->CallObjectMethod(evtHandler, getClassId);
    jclass handlerClassReflCls = env->GetObjectClass(handlerClassRefl);

    jmethodID getNameId = env->GetMethodID(handlerClassReflCls, "getName", "()Ljava/lang/String;");
    jstring classFullName = reinterpret_cast<jstring>(env->CallObjectMethod(handlerClassRefl, getNameId));
    jmethodID hashCodeId = env->GetMethodID(handlerClass, "hashCode", "()I");
    jint hashCode = env->CallIntMethod(evtHandler, hashCodeId);

    std::string fullName = env->GetStringUTFChars(classFullName, NULL);
    auto hashCodeStr = StringHelpers::to_string(hashCode);
    std::string key;
    key.append(evtName);
    key.append(":");
    key.append(fullName);
    key.append(":");
    key.append(hashCodeStr);

    return key;
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_emit(
    JNIEnv* env,
    jobject thisObj,
    jstring evtName,
    jobject evtData)
{
    throw std::exception();
}

JNIEXPORT jstring JNICALL Java_com_voicemynews_core_bindings_events_EventLoopBindingNative_on(
    JNIEnv* env,
    jobject thisObj,
    jstring evtName,
    jobject evtHandler)
{
    std::unique_lock<std::mutex> listenersLock;
    jclass handlerClass = env->GetObjectClass(evtHandler);
    std::string evtNameStd = env->GetStringUTFChars(evtName, NULL);
    auto handlerKey = GetStrForEventHandlerPtr(env, evtNameStd, handlerClass, evtHandler);

    /*auto evtHandlerPtr = eventLoop_.On(evtNameStd, std::function<void(std::shared_ptr<EventLoopBinding::EventData>)>([handler](std::shared_ptr<EventLoopBinding::EventData> evtDataStd) {
        auto evtData = EventLoopPlatform::BuildEvent(ConvertStdStrToPlatform(evtDataStd->data()));
        handler(evtData);
    }));

    registeredListeners_[handlerKey] = ListenerModel(evtName, evtHandlerPtr);*/

    return env->NewStringUTF(handlerKey.c_str());
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
    if (CurrVM == nullptr) {
        env->GetJavaVM(&CurrVM);
    }

    if (EmitterCls == nullptr) {
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