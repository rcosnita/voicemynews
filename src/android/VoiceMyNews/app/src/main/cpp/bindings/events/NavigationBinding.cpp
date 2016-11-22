#include "NavigationBinding.h"

#include <exception>

using namespace v8;

/**
 * \brief Provides the js wrapper for navigating to app sections using native implementation.
 */
static void NavigateJsNavigationManagerByEvent(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input parameters.
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
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
}
}
}
}
}