#include "pch.h"

#include <jsrt.h>

#include "CppUnitTest.h"
#include "utils/ChakraRunner.h"
#include "utils/CommonAssertions.h"

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Platform::String;

using voicemynews::tests::app::win10::utils::assertions::AssertNoJsError;
using voicemynews::tests::app::win10::utils::js::ChakraRunner;

/**
 * \class HttpChakraIntegrationTests
 * \brief This class provides the tests which proves http module under chakra works as expected.
 */
TEST_CLASS(HttpChakraIntegrationTests) {
public:
    TEST_METHOD_INITIALIZE(SetUp) {
    }

    TEST_METHOD_CLEANUP(TearDown) {
    }

    /**
     * \brief This test case ensures http module loads as expected under chakra.
     */
    TEST_METHOD(HttpChakraModuleLoadedOk) {
        String^ jsScript = "(() => {const http = require('js/networking/http'); return http; })()";

        chakraRunner_.RunScript(jsScript, L"", [](const JsErrorCode& jsErrorCode, const JsValueRef& result, ChakraRunner* runner) {
            AssertNoJsError(jsErrorCode);

            auto undefinedObj = runner->GetUndefinedValue();
            Assert::AreNotEqual(undefinedObj, result);
        });
    }

private:
    ChakraRunner chakraRunner_ = ChakraRunner(true);
};
}
}
}
}