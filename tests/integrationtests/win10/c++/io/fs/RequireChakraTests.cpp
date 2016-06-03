#include "pch.h"

#include <chrono>
#include <jsrt.h>
#include <vector>

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
        AssertNoJsError(jsErrorCode);
    }

    TEST_METHOD_CLEANUP(TearDown) {
        JsSetCurrentContext(JS_INVALID_REFERENCE);
        JsDisposeRuntime(runtime_);
    }

    /**
     * \brief This test case checks if require can load modules with extension.
     */
    TEST_METHOD(RequireChakraEmbeddedOkJsExtensionProvided) {
        CheckRequireChakraEmbeddedOkTemplate("js/samples/dummymodule.js");
    }

    /**
    * \brief This test case checks if require can load modules without extension.
    */
    TEST_METHOD(RequireChakraEmbeddedOkJsExtensionNotProvided) {
        CheckRequireChakraEmbeddedOkTemplate("js/samples/dummymodule");
    }

    /**
     * \brief This test case checks if require fails if no module name is provided.
     */
    TEST_METHOD(RequireChakraEmbeddedOkNoModuleNameProvided) {
        std::vector<std::wstring> scriptSources = {
            L"(() => { const dummy = require(); return dummy; })();",
            L"(() => { const dummy = require(null); return dummy; })();",
            L"(() => { const dummy = require(""); return dummy; })();",
            L"(() => { const dummy = require('   '); return dummy; })();"
        };

        for (auto it = scriptSources.begin(); it != scriptSources.end(); it++) {
            auto jsScript = it->c_str();

            JsValueRef result;
            auto jsErrorCode = JsRunScript(jsScript, currentSourceContext_++, L"", &result);
            AssertNoJsError(jsErrorCode);

            JsValueRef undefinedObj ;
            jsErrorCode = JsGetUndefinedValue(&undefinedObj);
            AssertNoJsError(jsErrorCode);

            Assert::AreEqual(undefinedObj, result);
        }
    }

    /**
     * \brief This test case triggers the load of a module twice in order to give require internal cache the change to work.
     */
    TEST_METHOD(RequireChakraEmbeddedOkTransparentCache) {
        const float kCacheExpectedBoost = 2;
        const int kSubsequentIterations = 1000;

        auto startTime = std::chrono::system_clock().now();
        CheckRequireChakraEmbeddedOkTemplate("js/samples/dummymodule");
        auto endTime = std::chrono::system_clock().now();
        auto firstDuration = endTime - startTime;
        long duration = 0;

        for (auto i = 0; i < kSubsequentIterations; i++) {
            startTime = std::chrono::system_clock().now();
            CheckRequireChakraEmbeddedOkTemplate("js/samples/dummymodule");
            endTime = std::chrono::system_clock().now();
            duration += (endTime - startTime).count();
        }

        Assert::IsTrue((duration / kSubsequentIterations) < (firstDuration.count() / kCacheExpectedBoost));
    }

private:
    /**
     * \brief This method provides a generic template for testing if require correctly loads existing modules.
     *
     * We are able to use this in order to see if module with and without js extension are correctly loaded.
     *
     * \param moduleName the relative path to AppX of the module we want to load.
     */
    void CheckRequireChakraEmbeddedOkTemplate(String^ moduleName) {
        auto testScriptSource = "(() => { const dummy = require('" + moduleName + "'); return dummy; })();";
        auto jsScript = testScriptSource->Data();

        JsValueRef result;
        auto jsErrorCode = JsRunScript(jsScript, currentSourceContext_++, L"", &result);
        AssertNoJsError(jsErrorCode);

        JsValueRef resultJSString;
        jsErrorCode = JsConvertValueToString(result, &resultJSString);
        AssertNoJsError(jsErrorCode);

        const wchar_t *resultWC;
        size_t stringLength;
        jsErrorCode = JsStringToPointer(resultJSString, &resultWC, &stringLength);
        AssertNoJsError(jsErrorCode);

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