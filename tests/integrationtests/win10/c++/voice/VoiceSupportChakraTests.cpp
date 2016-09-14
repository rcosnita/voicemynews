#include "pch.h"
#include <jsrt.h>

#include "CppUnitTest.h"
#include "utils/ChakraRunner.h"
#include "utils/CommonAssertions.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Platform::String;

using voicemynews::tests::app::win10::utils::assertions::AssertNoJsError;
using voicemynews::tests::app::win10::utils::js::ChakraRunner;

namespace voicemynews {
namespace tests {
namespace integrationtesting {
namespace app {
/**
 * \class VoiceSupportChakraTests
 * \brief This class provides the integration tests for voice support binding projected into chakra js engine.
 */
TEST_CLASS(VoiceSupportChakraTests)
{
public:
    TEST_METHOD_INITIALIZE(SetUp) { }

    TEST_METHOD_CLEANUP(TearDown) { }

    /**
     * \brief This test case ensures voice support instances can be obtained correctly within js logic.
     */
    TEST_METHOD(VoiceSupportChakraTestsVoiceSupportProjectedOk)
    {
        String^ jsScript = "(() => {const voiceSupport = voicemynews.core.voice.VoiceSupport.getInstance(); return voiceSupport; })()";

        chakraRunner_.RunScript(jsScript, L"", [](const JsErrorCode& jsErrorCode, const JsValueRef& result, ChakraRunner* runner) {
            AssertNoJsError(jsErrorCode);

            auto undefinedObj = runner->GetUndefinedValue();
            Assert::AreNotEqual(undefinedObj, result);
        });
    }

    /**
     * \brief This test case ensures voice support notifications can be obtained correctly within js logic.
     */
    TEST_METHOD(VoiceSupportChakraTestsVoiceNotificationsProjectedOk)
    {
        String^ jsScript = "(() => {const playerNotifications = voicemynews.core.voice.VoiceSupport.getNotificationsInstance((currPos) => {}, (currPos) => {}, (currPos) => {}, () => {}); return playerNotifications; })()";

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