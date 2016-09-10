#include "pch.h"

#include <chrono>
#include <jsrt.h>
#include <vector>

#include "CppUnitTest.h"
#include "io/fs/FileUtils.h"
#include "utils/ChakraRunner.h"
#include "utils/CommonAssertions.h"

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Platform::String;

using voicemynews::core::io::fs::FileUtils;
using voicemynews::tests::app::win10::utils::js::ChakraRunner;
using voicemynews::tests::app::win10::utils::assertions::AssertNoJsError;

/**
 * \class RequireChakraIntegrationTests
 * \brief This class provides the integration tests for making sure that requirejs works as expected.
 */
TEST_CLASS(RequireChakraIntegrationTests) {
public:
    TEST_METHOD_INITIALIZE(SetUp) {
    }

    TEST_METHOD_CLEANUP(TearDown) {
    }

    /**
     * \brief This test case checks if require can load modules with extension.
     */
    TEST_METHOD(RequireChakraIntegrationTestsRequireChakraEmbeddedOkJsExtensionProvided) {
        CheckRequireChakraEmbeddedOkTemplate("js/samples/dummymodule.js");
    }

    /**
    * \brief This test case checks if require can load modules without extension.
    */
    TEST_METHOD(RequireChakraIntegrationTestsRequireChakraEmbeddedOkJsExtensionNotProvided) {
        CheckRequireChakraEmbeddedOkTemplate("js/samples/dummymodule");
    }

    /**
     * \brief This test case checks if require fails if no module name is provided.
     */
    TEST_METHOD(RequireChakraIntegrationTestsRequireChakraEmbeddedOkNoModuleNameProvided) {
        std::vector<String^> scriptSources = {
            L"(() => { const dummy = require(); return dummy; })();",
            L"(() => { const dummy = require(null); return dummy; })();",
            L"(() => { const dummy = require(""); return dummy; })();",
            L"(() => { const dummy = require('   '); return dummy; })();"
        };

        for (auto it = scriptSources.begin(); it != scriptSources.end(); it++) {
            chakraRunner_.RunScript(*it, L"", [](const JsErrorCode& jsErrorCode, const JsValueRef& result, ChakraRunner* runner) {
                AssertNoJsError(jsErrorCode);

                JsValueRef undefinedObj = runner->GetUndefinedValue();
                Assert::AreEqual(undefinedObj, result);
            });
        }
    }

    /**
     * \brief This test case triggers the load of a module twice in order to give require internal cache the change to work.
     */
    TEST_METHOD(RequireChakraIntegrationTestsRequireChakraEmbeddedOkTransparentCache) {
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
            duration += static_cast<long>((endTime - startTime).count());
        }

        Assert::IsTrue((duration / kSubsequentIterations) < (firstDuration.count() / kCacheExpectedBoost));
    }

    /**
     * \brief This test case ensures requireRaw works as expected for existing files.
     */
    TEST_METHOD(RequireChakraIntegrationTestsRequireRawOk) {
        std::wstring expectedContent = fileUtils_.ReadFile("js/samples/raw_sample.txt");
        String^ jsScript = "(() => {const content = requireRaw('js/samples/raw_sample.txt'); return content;})();";
        chakraRunner_.RunScript(jsScript, L"", [&expectedContent](const JsErrorCode jsErrorCode, const JsValueRef result, ChakraRunner* runner) {
            AssertNoJsError(jsErrorCode);

            JsValueRef resultJSString;
            auto jsError = runner->ConvertValueToString(result, &resultJSString);
            AssertNoJsError(jsError);

            const wchar_t *resultWC;
            size_t stringLength;
            jsError = runner->ConvertStringToPointer(resultJSString, &resultWC, &stringLength);
            AssertNoJsError(jsError);

            Assert::AreEqual(0, expectedContent.compare(std::wstring(resultWC)));
        });
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
        auto jsScript = "(() => { const dummy = require('" + moduleName + "'); return dummy; })();";

        chakraRunner_.RunScript(jsScript, moduleName, [](const JsErrorCode& jsErrorCode, const JsValueRef& result, ChakraRunner* runner) {
            AssertNoJsError(jsErrorCode);

            JsValueRef resultJSString;
            auto jsError = runner->ConvertValueToString(result, &resultJSString);
            AssertNoJsError(jsError);

            const wchar_t *resultWC;
            size_t stringLength;
            jsError = runner->ConvertStringToPointer(resultJSString, &resultWC, &stringLength);
            AssertNoJsError(jsError);

            Assert::AreEqual(L"Hello world", resultWC);
        });
    }

private:
    ChakraRunner chakraRunner_ = ChakraRunner(true);
    FileUtils fileUtils_;
};
}
}
}
}