#include "pch.h"

#include <jsrt.h>
#include <chrono>
#include <thread>

#include "CppUnitTest.h"
#include "events/EventNames.h"
#include "utils/ChakraRunner.h"
#include "utils/CommonAssertions.h"
#include "utils/Conversions.h"

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
using namespace std::chrono_literals;

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Platform::String;

using voicemynews::core::events::EventLoopPlatform;
using voicemynews::tests::app::win10::utils::assertions::AssertNoJsError;
using voicemynews::tests::app::win10::utils::js::ChakraRunner;
using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::bindings::events::EventHandler;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;

/**
 * \class JsAppIntegrationTests
 * \brief This class provides the tests which proves that main app.js module runs correctly.
 */
TEST_CLASS(JsAppIntegrationTests) {
public:
    TEST_METHOD_INITIALIZE(SetUp) {
    }

    TEST_METHOD_CLEANUP(TearDown) {
    }

    /**
     * This test case proves that app.js loads correctly, starts an event loop and shutdown when necessary.
     */
    TEST_METHOD(JsAppLoadsCorrectly) {
        auto eventLoop = EventLoopPlatform::GetInstance();
        std::thread shutdownEmitter;

        eventLoop->On(ConvertStdStrToPlatform(voicemynews::core::events::kAppJsStart),
            ref new EventHandler([&eventLoop, &shutdownEmitter](EventDataBinding^ evtData) {
            shutdownEmitter = std::thread([&eventLoop]() {
                std::this_thread::sleep_for(100ms);
                eventLoop->Emit(ConvertStdStrToPlatform(voicemynews::core::events::kAppJsShutdown),
                    EventLoopPlatform::BuildEvent("{}"));
            });
        }));

        String^ jsScript = "(() => { const app = require('js/app'); })();";

        chakraRunner_.RunScript(jsScript, L"", [&shutdownEmitter](const JsErrorCode& jsErrorCode, const JsValueRef& result, ChakraRunner* runner) {
            AssertNoJsError(jsErrorCode);

            auto undefinedObj = runner->GetUndefinedValue();
            Assert::AreEqual(undefinedObj, result);

            shutdownEmitter.join();
        });
    }

private:
    ChakraRunner chakraRunner_ = ChakraRunner(true);
};
}
}
}
}