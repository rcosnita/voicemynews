#include "pch.h"
#include "CppUnitTest.h"

#include "voice/VoiceSupport.h"
#include <ppltasks.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace voice {
/**
 * \brief Provides the tests suite for voice support implementation.
 */
TEST_CLASS(VoiceSupportTests)
{
public:
    TEST_METHOD(VoiceSupportTestsFactoryMethodOk)
    {
        auto voiceSupport = voicemynews::core::voice::GetVoiceSupportInstance();

        Assert::IsNotNull(voiceSupport.get());
    }
};
}
}
}
}