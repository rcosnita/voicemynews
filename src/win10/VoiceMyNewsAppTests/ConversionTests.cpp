#include "pch.h"
#include "CppUnitTest.h"
#include "utils/Conversions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace voicemynews::app::win10::utils;
using Platform::Collections::Map;
using Platform::String;
using Windows::Foundation::Collections::IMap;

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

    TEST_METHOD(ConversionTestsConvertPlatformMapToStd) {
        auto map = ref new Map<String^, String^>();
        map->Insert("key1", "Value1");
        map->Insert("key2", "value2");

        auto result = ConvertPlatformMapToStd(map);
        
        Assert::AreEqual(2, static_cast<int>(result.size()));
        Assert::AreEqual(0, result["key1"].compare(std::string("Value1")));
        Assert::AreEqual(0, result["key2"].compare(std::string("value2")));
    }

    TEST_METHOD(ConversionTestsConvertPlatformMapToStdInterfaceOk) {
        IMap<String^, String^>^ map = ref new Map<String^, String^>();
        map->Insert("key1", "Value1");
        map->Insert("key2", "value2");

        auto result = ConvertPlatformMapToStd(map);

        Assert::AreEqual(2, static_cast<int>(result.size()));
        Assert::AreEqual(0, result["key1"].compare(std::string("Value1")));
        Assert::AreEqual(0, result["key2"].compare(std::string("value2")));
    }

    TEST_METHOD(ConversionTestsConvertPlatformMapToStdNullPtr) {
        Map<String^, String^>^ map = nullptr;
        auto result = ConvertPlatformMapToStd(map);

        Assert::AreEqual(0, static_cast<int>(result.size()));
    }

    TEST_METHOD(ConversionTestsConvertStdMapToPlatformOk) {
        std::map<std::string, std::string> input;
        input["key1"] = "value1";
        input["key2"] = "Value2";

        auto output = ConvertStdMapToPlatform(input);

        Assert::IsNotNull(output);
        Assert::AreEqual(2, static_cast<int>(output->Size));
        Assert::AreEqual("value1", output->Lookup("key1"));
        Assert::AreEqual("Value2", output->Lookup("key2"));
    }

    TEST_METHOD(ConversionTestsConvertStdMapToPlatformUninitialized) {
        std::map<std::string, std::string> input;

        auto output = ConvertStdMapToPlatform(input);

        Assert::IsNotNull(output);
        Assert::AreEqual(0, static_cast<int>(output->Size));
    }
};

}
}
}
}