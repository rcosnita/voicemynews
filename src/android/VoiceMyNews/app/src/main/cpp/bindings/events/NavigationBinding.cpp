#include "events/EventData.h"
#include "NavigationBinding.h"

#include <exception>
#include <jni.h>
#include <string>

using namespace v8;
using voicemynews::core::events::EventData;

static JavaVM* CurrJavaVM = nullptr;
static jobject NavigationBindingObj = nullptr;
static jmethodID NavigateByEventMethod = nullptr;

/**
 * \brief Provides the js wrapper for navigating to app sections using native implementation.
 */
static void NavigateJsNavigationManagerByEvent(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input parameters.
    Isolate* isolate = info.GetIsolate();
    String::Utf8Value evtName(info[0]->ToString());
    std::string evtNameStd(*evtName);
    EventData<std::string>* eventObj = reinterpret_cast<EventData<std::string>*>(Local<External>::Cast(info[1]->ToObject()->GetInternalField(0))->Value());
    std::string evtData = eventObj->data();

    JNIEnv* env = nullptr;
    CurrJavaVM->AttachCurrentThread(&env, nullptr);
    jstring evtNameVM = env->NewStringUTF(evtNameStd.c_str());
    jstring evtDataVM = env->NewStringUTF(evtData.c_str());

    env->CallVoidMethod(NavigationBindingObj, NavigateByEventMethod, evtNameVM, evtDataVM);
}

/**
 * \brief Provides the js wrapper code which can return a navigation manager instance.
 */
static void GetJsNavigationManagerInstance(const FunctionCallbackInfo<Value>& info)
{
    auto isolate = info.GetIsolate();
    Local<ObjectTemplate> obj = ObjectTemplate::New(isolate);
    obj->Set(isolate, "navigateByEvent", FunctionTemplate::New(isolate, NavigateJsNavigationManagerByEvent));

    auto objInst = obj->NewInstance();
    info.GetReturnValue().Set(objInst);
}

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace events {
void NavigationBinding::WireToJs(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> obj)
{
    Local<ObjectTemplate> navManagerPlatform = ObjectTemplate::New(isolate);
    obj->Set(isolate, "NavigationManagerPlatform", navManagerPlatform);

    navManagerPlatform->Set(isolate, "getInstance", FunctionTemplate::New(isolate, GetJsNavigationManagerInstance));
}

void InitNavigationBinding(JNIEnv* env)
{
    env->GetJavaVM(&CurrJavaVM);

    auto navigationBindingCls = env->FindClass("com/voicemynews/core/bindings/events/NavigationBinding");
    auto getInstanceMethod = env->GetStaticMethodID(navigationBindingCls, "getInstance", "()Lcom/voicemynews/core/bindings/events/NavigationBinding;");
    NavigateByEventMethod = env->GetMethodID(navigationBindingCls, "navigateByEvent", "(Ljava/lang/String;Ljava/lang/String;)V");
    NavigationBindingObj = env->NewGlobalRef(env->CallStaticObjectMethod(navigationBindingCls, getInstanceMethod));
}
}
}
}
}
}