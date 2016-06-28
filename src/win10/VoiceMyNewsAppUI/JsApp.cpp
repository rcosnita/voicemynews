#include "pch.h"
#include "JsApp.h"

/**
 * \brief This method represent boiler plate code required by Chakra in order to be able to correctly handle promises continuation.
 */
static void CALLBACK PromiseContinuationCallback(JsValueRef task, void *callbackState)
{
    *(void **)callbackState = task;
}

namespace voicemynews {
namespace app {
namespace win10 {
namespace js {
using Platform::String;

JsApp::JsApp() {
    currentSourceContext_ = 0;

    JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime_);
    JsCreateContext(runtime_, &context_);
    JsSetCurrentContext(context_);

    JsProjectWinRTNamespace(L"voicemynews.app.win10.bindings");
    JsProjectWinRTNamespace(L"voicemynews.core.network");
    JsProjectWinRTNamespace(L"voicemynews.core.events");

    JsSetPromiseContinuationCallback(PromiseContinuationCallback, &promiseCallback_);
#ifdef _DEBUG
    JsStartDebugging();
#endif
}

JsApp::~JsApp() {
    JsSetCurrentContext(JS_INVALID_REFERENCE);
    JsDisposeRuntime(runtime_);
}

void JsApp::Start() {
    BindRequireJs();
    StartApp();
}

void JsApp::BindRequireJs() {
    auto requireSource = fileUtils_.ReadFile("js/require.js");
    JsRunScript(requireSource.c_str(), currentSourceContext_++, L"js/require.js", nullptr);
}

void JsApp::StartApp() {
    String^ appScript = "(() => { const app = require('js/app'); })();";
    JsRunScript(appScript->Data(), currentSourceContext_++, L"js/app.js", nullptr);
}

}
}
}
}