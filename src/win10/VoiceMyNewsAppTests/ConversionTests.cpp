#include "pch.h"
#include "CppUnitTest.h"
#include "utils/Conversions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace voicemynews::app::win10::utils;
using Platform::String;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace utils {
/**
 * \brief This class provides the unit tests for conversion methods between std and winrt types.
 */
TEST_CLASS(ConversionTests) {
public:
    TEST_METHOD(ConversionTestsConvertPlatformStrToStdOk) {
        String^ input = "hello world ...";
        std::string expectedOutput = "hello world ...";

        std::string output = ConvertPlatformStrToStd(input);

        Assert::AreEqual(0, expectedOutput.compare(output));
    }

    TEST_METHOD(ConversionTestsConvertPlatformStrToStdNullPtr) {
        String^ input = nullptr;
        std::string expectedOutput = "";

        std::string output = ConvertPlatformStrToStd(input);

        Assert::AreEqual(0, expectedOutput.compare(expectedOutput));
    }

    TEST_METHOD(ConversionTestsConvertStdStrToPlatformOk) {
        std::string input = "hello world ...";
        String^ expectedOutput = "hello world ...";

        String^ output = ConvertStdStrToPlatform(input);

        Assert::AreEqual(expectedOutput, output);
    }

    TEST_METHOD(ConversionTestsConvertStdStrToPlatformEmptyString) {
        std::string input = "";

        String^ output = ConvertStdStrToPlatform(input);

        Assert::IsNull(output);
    }

    TEST_METHOD(ConversionTestsConvertStdStrToPlatformUninitializedString) {
        std::string input;

        String^ output = ConvertStdStrToPlatform(input);

        Assert::IsNull(output);
    }

    TEST_METHOD(ConversionTestsConvertStdStrToPlatformWideOk) {
        std::wstring input = L"hello world ...";
        String^ expectedOutput = "hello world ...";

        String^ output = ConvertStdStrToPlatform(input);

        Assert::AreEqual(expectedOutput, output);
    }

    TEST_METHOD(ConversionTestsConvertStdStrToPlatformWideEmptyString) {
        std::wstring input = L"";

        String^ output = ConvertStdStrToPlatform(input);

        Assert::IsNull(output);
    }

    TEST_METHOD(ConversionTestsConvertStdStrToPlatformWideUninitializedString) {
        std::wstring input;

        String^ output = ConvertStdStrToPlatform(input);

        Assert::IsNull(output);
    }
};

}
}
}
}