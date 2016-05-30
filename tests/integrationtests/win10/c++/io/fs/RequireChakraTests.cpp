#include "pch.h"

#include <jsrt.h>

#include "CppUnitTest.h"

#include "io/fs/FileUtils.h"

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Platform::String;

using voicemynews::core::io::fs::FileUtils;

/**
 * \class RequireChakraIntegrationTests
 * \brief This class provides the integration tests for making sure that requirejs works as expected.
 */
TEST_CLASS(RequireChakraIntegrationTests) {
public:
    TEST_METHOD_INITIALIZE(SetUp) {
        currentSourceContext_ = 0;

        JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime_);
        JsCreateContext(runtime_, &context_);
        JsSetCurrentContext(context_);

        JsProjectWinRTNamespace(L"voicemynews.app.win10.bindings");
#ifdef _DEBUG
        JsStartDebugging();
#endif

        auto requireSource = fileUtils_.ReadFile("js/require.js");
        auto jsErrorCode = JsRunScript(requireSource.c_str(), currentSourceContext_++, L"js/require.js", nullptr);
        Assert::AreEqual(static_cast<int>(JsErrorCode::JsNoError), static_cast<int>(jsErrorCode));
    }

    TEST_METHOD_CLEANUP(TearDown) {
        JsSetCurrentContext(JS_INVALID_REFERENCE);
        JsDisposeRuntime(runtime_);
    }

    /**
     * \brief This test case provides the logic for verifying requirejs implementation inside chakra js engine.
     */
    TEST_METHOD(RequireChakraEmbeddedOk) {
        const wchar_t *jsScript = L"(() => { const dummy = require('js/samples/dummymodule.js'); return dummy; })();";
        JsValueRef result;
        auto jsErrorCode = JsRunScript(jsScript, currentSourceContext_++, L"", &result);
        Assert::AreEqual(static_cast<int>(JsErrorCode::JsNoError), static_cast<int>(jsErrorCode));

        JsValueRef resultJSString;
        jsErrorCode = JsConvertValueToString(result, &resultJSString);
        Assert::AreEqual(static_cast<int>(JsErrorCode::JsNoError), static_cast<int>(jsErrorCode));

        const wchar_t *resultWC;
        size_t stringLength;
        jsErrorCode = JsStringToPointer(resultJSString, &resultWC, &stringLength);
        Assert::AreEqual(static_cast<int>(JsErrorCode::JsNoError), static_cast<int>(jsErrorCode));

        Assert::AreEqual(L"Hello world", resultWC);
    }

private:
    JsRuntimeHandle runtime_;
    JsContextRef context_;
    FileUtils fileUtils_;
    unsigned int currentSourceContext_;
};
}
}
}
}