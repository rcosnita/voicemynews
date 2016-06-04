#include "pch.h"
#include "ChakraRunner.h"
#include "CommonAssertions.h"

namespace voicemynews {
namespace tests {
namespace app {
namespace win10 {
namespace utils {
namespace js {
using voicemynews::tests::app::win10::utils::assertions::AssertNoJsError;

    ChakraRunner::ChakraRunner(bool enableRequire) {
        currentSourceContext_ = 0;

        JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime_);
        JsCreateContext(runtime_, &context_);
        JsSetCurrentContext(context_);

        JsProjectWinRTNamespace(L"voicemynews.app.win10.bindings");
#ifdef _DEBUG
        JsStartDebugging();
#endif
        if (enableRequire) {
            auto requireSource = fileUtils_.ReadFile("js/require.js");
            auto jsErrorCode = JsRunScript(requireSource.c_str(), currentSourceContext_++, L"js/require.js", nullptr);
            AssertNoJsError(jsErrorCode);
        }
    }

    ChakraRunner::~ChakraRunner() {
        JsSetCurrentContext(JS_INVALID_REFERENCE);
        JsDisposeRuntime(runtime_);
    }

    JsValueRef ChakraRunner::RunScript(String^ jsScript, String^ sourceName, JsScriptRunnerCb handleResult) {
        JsValueRef result;
        auto jsErrorCode = JsRunScript(jsScript->Data(), currentSourceContext_++, sourceName->Data(), &result);

        if (handleResult == nullptr) {
            return nullptr;
        }

        handleResult(jsErrorCode, result, this);

        return result;
    }

    JsErrorCode ChakraRunner::ConvertValueToString(JsValueRef value, JsValueRef *result) {
        return JsConvertValueToString(value, result);
    }

    JsErrorCode ChakraRunner::ConvertStringToPointer(JsValueRef value, const wchar_t **str, size_t *stringLength) {
        return JsStringToPointer(value, str, stringLength);
    }

    JsValueRef ChakraRunner::GetUndefinedValue() {
        JsValueRef undefinedObj;
        auto jsError = JsGetUndefinedValue(&undefinedObj);
        AssertNoJsError(jsError);

        return undefinedObj;
    }
}
}
}
}
}
}