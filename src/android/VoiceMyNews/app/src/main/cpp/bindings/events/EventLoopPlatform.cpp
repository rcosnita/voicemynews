#include "EventLoopBinding.h"
#include "EventLoopPlatform.h"
#include "utils/StringHelpers.h"

#include <functional>
#include <memory>

using namespace v8;

using voicemynews::core::events::EventData;
using voicemynews::app::android::bindings::events::EventDataBinding;
using voicemynews::app::android::utils::StringHelpers;

static const char* EventHandlerActionMethodSig = "(Lcom/voicemynews/core/bindings/events/EventDataBindingNative;)V";
static const char* EventHandlerActionMethodName = "handleEvent";

static jclass EventDataBindingNativeCls = nullptr;

static voicemynews::core::events::EventLoopPlatform* EventLoopInstance = nullptr;
static Persistent<Object> *JsLoopInstance = nullptr;

/**
 * \brief Provides the js wrapper which can obtain js events wrapped around native events.
 */
static void BuildJsLoopEvent(const FunctionCallbackInfo<Value> &info)
{
    // TODO [rcosnita] validate input parameters.
    Isolate* isolate = info.GetIsolate();

    Local<ObjectTemplate> obj = ObjectTemplate::New(isolate);
    obj->SetInternalFieldCount(1);

    Local<Object> objInstance = obj->NewInstance();
    auto evtData = new EventData<std::string>(*String::Utf8Value(info[0]->ToString()));
    objInstance->SetInternalField(0, External::New(isolate, evtData));
    info.GetReturnValue().Set(objInstance);
}

/**
 * \brief Provides the logic for obtaining js event data from the current object.
 */
static void GetJsEvtData(Local<String> property, const PropertyCallbackInfo<Value>& info)
{
    auto self = info.Holder();
    auto evtData = reinterpret_cast<EventData<std::string>*>(Local<External>::Cast(self->GetInternalField(0))->Value());
    auto jsEvtData = String::NewFromUtf8(info.GetIsolate(), evtData->data().c_str());
    info.GetReturnValue().Set(jsEvtData);
}

/**
 * \brief Provides the js wrapper which can wire js callbacks to native loop.
 */
static void OnJsLoop(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input parameters.
    Isolate* isolate = info.GetIsolate();
    Local<Object> self = info.Holder();
    std::string evtName = *(String::Utf8Value(info[0]->ToString()));
    auto callback = Local<Function>::Cast(info[1]);
    auto callbackPersistent = new Persistent<Function>(isolate, callback);
    auto eventLoopPtr = Local<External>::Cast(self->GetInternalField(0))->Value();
    auto eventLoop = reinterpret_cast<voicemynews::core::events::EventLoopPlatform*>(eventLoopPtr);

    auto evtHandler = std::function<void(std::shared_ptr<voicemynews::core::events::EventData<std::string>>)>(
        [&callbackPersistent, &isolate](std::shared_ptr<voicemynews::core::events::EventData<std::string>> evtData) {
        Local<Function> callback = callbackPersistent->Get(isolate);
        Local<Value> args[1];
        Local<ObjectTemplate> objEvtData = ObjectTemplate::New(isolate);
        objEvtData->SetInternalFieldCount(1);
        objEvtData->SetAccessor(String::NewFromUtf8(isolate, "evtData"), GetJsEvtData);

        Local<Object> evt = objEvtData->NewInstance();
        evt->SetInternalField(0, External::New(isolate, evtData.get()));
        args[0] = evt;

        callback->Call(callback, 1, args);
    });

    auto eventLoopCasted = dynamic_cast<voicemynews::core::events::EventLoop*>(eventLoop);
    eventLoopCasted->On(evtName, evtHandler);
}

/**
 * \brief Provides the js wrapper which can unregister js callbacks from native loop.
 */
static void OffJsLoop(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input parameters.
    throw std::exception();
}

/**
 * \brief Provides the js wrapper which can emit events from business logic to native app.
 */
static void EmitJsLoop(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input parameters.
    Local<Object> self = info.Holder();
    std::string evtName = *String::Utf8Value(info[0]->ToString());
    auto evtDataPtr = Local<External>::Cast(info[1]->ToObject()->GetInternalField(0))->Value();
    auto evtData = reinterpret_cast<EventData<std::string>*>(evtDataPtr);
    auto eventLoopPtr = Local<External>::Cast(self->GetInternalField(0))->Value();
    auto eventLoop = reinterpret_cast<voicemynews::core::events::EventLoopPlatform*>(eventLoopPtr);
    auto eventLoopCasted = dynamic_cast<voicemynews::core::events::EventLoop*>(eventLoop);

    eventLoopCasted->Emit(evtName, std::shared_ptr<EventData<std::string>>(evtData));
}

/**
 * \brief Provides the js wrapper which can process all pending actions accumulated from the last process tick.
 */
static void ProcessEventsJsLoop(const FunctionCallbackInfo<Value>& info)
{
    auto holder = info.Holder();
    auto eventLoop = reinterpret_cast<voicemynews::core::events::EventLoopPlatform*>(Local<External>::Cast(holder->GetInternalField(0))->Value());
    eventLoop->ProcessEvents();
}

/**
 * \brief Provides a js wrapper over EventLoopPlatform getInstance factory method.
 */
static void GetJsLoopInstance(const FunctionCallbackInfo<Value> &info)
{
    Isolate* isolate = info.GetIsolate();

    if (JsLoopInstance != nullptr)
    {
        auto loop = JsLoopInstance->Get(isolate);
        info.GetReturnValue().Set(loop);

        return;
    }

    auto loopObj = ObjectTemplate::New(isolate);
    loopObj->SetInternalFieldCount(1);
    loopObj->Set(isolate, "on", FunctionTemplate::New(isolate, OnJsLoop));
    loopObj->Set(isolate, "off", FunctionTemplate::New(isolate, OffJsLoop));
    loopObj->Set(isolate, "emit", FunctionTemplate::New(isolate, EmitJsLoop));
    loopObj->Set(isolate, "processEvents", FunctionTemplate::New(isolate, ProcessEventsJsLoop));

    auto loop = loopObj->NewInstance();
    auto nativeLoop = voicemynews::core::events::EventLoopPlatform::GetInstance();

    loop->SetInternalField(0, External::New(isolate, nativeLoop));

    info.GetReturnValue().Set(loop);
    JsLoopInstance = new Persistent<Object>(isolate, loop);
}

namespace voicemynews {
namespace core {
namespace events {
EventLoopPlatform::EventLoopPlatform(bool processImmediate)
    : processImmediate_(processImmediate)
{
}

EventLoopPlatform* EventLoopPlatform::GetInstance()
{
    if (EventLoopInstance == nullptr) {
        EventLoopInstance = new EventLoopPlatform(false);
    }

    return EventLoopInstance;
}

jobject EventLoopPlatform::BuildEvent(JNIEnv* env, std::string data)
{
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

    if (EventDataBindingNativeCls == nullptr) {
        auto eventDataBindingCls = env->FindClass("com/voicemynews/core/bindings/events/EventDataBindingNative");
        EventDataBindingNativeCls = (jclass)env->NewGlobalRef(eventDataBindingCls);
    }

    auto handler = std::function<void(std::shared_ptr<EventLoopPlatform::EventData>)>([this, handlerActionId, evtHandlerGlobal](std::shared_ptr<EventLoopPlatform::EventData> evtDataStd) {
        JNIEnv *env = nullptr;
        javaVM_->AttachCurrentThread(&env, nullptr);
        auto platformEvt = EventLoopPlatform::BuildEvent(env, evtDataStd->data());
        env->CallVoidMethod(evtHandlerGlobal, handlerActionId, platformEvt);
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

    while (!deferredTasks_.empty()) {
        auto task = deferredTasks_.back();
        task();
        deferredTasks_.pop();
    }
}

void EventLoopPlatform::EnqueueTask(EventLoopJsTask task)
{
    deferredTasks_.push(task);
}

void EventLoopPlatform::WireToJs(Isolate* isolate, Local<ObjectTemplate> obj)
{
    auto jsEventLoopPlatform = ObjectTemplate::New(isolate);
    jsEventLoopPlatform->Set(isolate, "buildEvent", FunctionTemplate::New(isolate, BuildJsLoopEvent));
    jsEventLoopPlatform->Set(isolate, "getInstance",
                             FunctionTemplate::New(isolate, GetJsLoopInstance));
    obj->Set(isolate, "EventLoopPlatform", jsEventLoopPlatform);
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