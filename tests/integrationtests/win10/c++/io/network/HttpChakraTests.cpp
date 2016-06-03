#include "pch.h"

#include <jsrt.h>

#include "CppUnitTest.h"
#include "io/fs/FileUtils.h"
#include "utils/CommonAssertions.h"

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Platform::String;

using voicemynews::core::io::fs::FileUtils;
using voicemynews::tests::app::win10::utils::assertions::AssertNoJsError;

/**
 * \class HttpChakraIntegrationTests
 * \brief This class provides the tests which proves http module under chakra works as expected.
 */
TEST_CLASS(HttpChakraIntegrationTests) {
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
        AssertNoJsError(jsErrorCode);
    }

    TEST_METHOD_CLEANUP(TearDown) {
        JsSetCurrentContext(JS_INVALID_REFERENCE);
        JsDisposeRuntime(runtime_);
    }

    /**
     * \brief This test case ensures http module loads as expected under chakra.
     */
    TEST_METHOD(HttpChakraModuleLoadedOk) {
        String^ testScriptSource = "(() => {const http = require('js/networking/http'); return http; })()";
        auto jsScript = testScriptSource->Data();

        JsValueRef undefinedObj;
        auto jsErrorCode = JsGetUndefinedValue(&undefinedObj);
        AssertNoJsError(jsErrorCode);

        JsValueRef result;
        jsErrorCode = JsRunScript(jsScript, currentSourceContext_++, L"", &result);
        AssertNoJsError(jsErrorCode);

        Assert::AreNotEqual(undefinedObj, result);
    }

    /**
     * \brief This test case ensures http module get operations work as expected under chakra.
     */
    TEST_METHOD(HttpChakraModuleGetOk) {
        String^ testScriptSource = "(() => {const http = require('js/networking/http'); return http.get('http://www.google.ro'); })()";
        auto jsScript = testScriptSource->Data();

        JsValueRef undefinedObj;
        auto jsErrorCode = JsGetUndefinedValue(&undefinedObj);
        AssertNoJsError(jsErrorCode);

        JsValueRef result;
        jsErrorCode = JsRunScript(jsScript, currentSourceContext_++, L"", &result);
        AssertNoJsError(jsErrorCode);

        Assert::AreNotEqual(undefinedObj, result);
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