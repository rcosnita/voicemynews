#include "EventLoopBinding.h"
#include "EventLoopPlatform.h"
#include "utils/StringHelpers.h"
#include "V8HelloWorld.h"

using voicemynews::app::android::bindings::events::EventDataBinding;
using voicemynews::app::android::utils::StringHelpers;

static const char* EventHandlerActionMethodSig = "(Lcom/voicemynews/core/bindings/events/EventDataBindingNative;)V";
static const char* EventHandlerActionMethodName = "handleEvent";

static jclass EventDataBindingNativeCls = nullptr;

namespace voicemynews {
namespace core {
namespace events {
EventLoopPlatform::EventLoopPlatform(bool processImmediate)
    : processImmediate_(processImmediate)
{
}

EventLoopPlatform* EventLoopPlatform::GetInstance()
{
    auto resultStr = voicemynews::app::android::RunV8HelloWorld();
    // TODO [rcosnita] Make sure we do not process events immediately.
    return new EventLoopPlatform(true);
}

jobject EventLoopPlatform::BuildEvent(JNIEnv* env, std::string data)
{
    if (EventDataBindingNativeCls == nullptr) {
        EventDataBindingNativeCls = (jclass)env->NewGlobalRef(env->FindClass("com/voicemynews/core/bindings/events/EventDataBindingNative"));
    }

    jstring evtDataJNI = env->NewStringUTF(data.c_str());
    return Java_com_voicemynews_core_bindings_events_EventDataBindingNative_getInstanceNative(env, EventDataBindingNativeCls, evtDataJNI);
}

std::string EventLoopPlatform::GetStrForEventHandlerPtr(JNIEnv *env, std::string evtName,
                                                        jclass handlerClass, jobject evtHandler)
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

jstring EventLoopPlatform::On(JNIEnv* env,
           jstring evtName,
           jobject evtHandler)
{
    std::unique_lock<std::mutex> listenersLock(registeredListenersMutex_);
    std::string evtNameStd = env->GetStringUTFChars(evtName, NULL);
    jclass handlerClass = env->GetObjectClass(evtHandler);
    auto handlerKey = GetStrForEventHandlerPtr(env, evtNameStd, handlerClass, evtHandler);
    auto handlerActionCls = env->GetObjectClass(evtHandler);
    auto handlerActionId = env->GetMethodID(handlerActionCls, EventHandlerActionMethodName, EventHandlerActionMethodSig);
    auto evtHandlerGlobal = env->NewGlobalRef(evtHandler);

    if (javaVM_ == nullptr) {
        env->GetJavaVM(&javaVM_);
    }

    auto handler = std::function<void(std::shared_ptr<EventLoopPlatform::EventData>)>([this, handlerActionId, evtHandlerGlobal](std::shared_ptr<EventLoopPlatform::EventData> evtDataStd) {
        JNIEnv *env = nullptr;
        javaVM_->AttachCurrentThread(&env, nullptr);
        auto platformEvt = EventLoopPlatform::BuildEvent(env, evtDataStd->data());
        env->CallVoidMethod(evtHandlerGlobal, handlerActionId, platformEvt);

        // TODO [rcosnita] uncomment this and make it more robust when the event loop is multithreaded.
//        javaVM_->DetachCurrentThread();
    });
    auto evtHandlerPtr = EventLoop::On(evtNameStd, handler);

    registeredListeners_[handlerKey] = ListenerModel(evtNameStd, evtHandlerPtr);

    return env->NewStringUTF(handlerKey.c_str());
}

void EventLoopPlatform::Off(std::string handlerKey)
{
    std::unique_lock<std::mutex> listenersLock(registeredListenersMutex_);
    auto entry = registeredListeners_.find(handlerKey);
    auto listenerModel = entry->second;
    EventLoop::Off(listenerModel.evtName, listenerModel.handlerPtr);
    registeredListeners_.erase(entry);
}

void EventLoopPlatform::Emit(JNIEnv* env,
          jstring evtName,
          jobject evtData)
{
    if (javaVM_ == nullptr) {
        env->GetJavaVM(&javaVM_);
    }

    std::string evtNameStd = env->GetStringUTFChars(evtName, nullptr);
    jclass evtDataCls = env->GetObjectClass(evtData);
    jfieldID  evtDataPtrId = env->GetFieldID(evtDataCls, "nativeEvtDataPtr", "J");
    jlong evtDataPtr = env->GetLongField(evtData, evtDataPtrId);
    auto evtDataNative = reinterpret_cast<voicemynews::app::android::bindings::events::EventDataBinding*>(evtDataPtr);

    EventLoop::Emit(evtNameStd, std::make_shared<EventLoopPlatform::EventData>(evtDataNative->data()));

    if (processImmediate_) {
        EventLoopPlatform::ProcessEvents();
    }
}

void EventLoopPlatform::ProcessEvents()
{
    EventLoop::ProcessEvents();

    // TODO [rcosnita] add support for deferred tasks execution.
    /*while (!deferredTasks_.empty()) {
        auto task = deferredTasks_.back();
        task();
        deferredTasks_.pop();
    }*/
}
}
}
}

JNIEXPORT jobject JNICALL Java_com_voicemynews_core_bindings_events_EventDataBindingNative_getInstanceNative(
    JNIEnv* env,
    jclass objCls,
    jstring evtData)
{
    auto constructorId = env->GetMethodID(objCls, "<init>", "(Ljava/lang/String;J)V");

    std::string evtDataStd = env->GetStringUTFChars(evtData, nullptr);
    auto nativeEvtData = new EventDataBinding(evtDataStd);
    jlong nativeEvtDataPtr = reinterpret_cast<uintptr_t>(nativeEvtData);

    return env->NewObject(objCls, constructorId, evtData, nativeEvtDataPtr);
}