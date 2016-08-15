#include "pch.h"
#include "CppUnitTest.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Platform::Object;
using Platform::String;

using voicemynews::app::win10::converters::JsonObjectConverter;

using Windows::Data::Json::IJsonObject;
using Windows::Data::Json::JsonObject;
using Windows::UI::Xaml::Data::IValueConverter;
using Windows::UI::Xaml::Interop::TypeName;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace converters {
/**
 * \brief This class provides the test cases which guarantees jsonobjectconverter works as expected in all scenarios.
 */
TEST_CLASS(JsonObjectConverterTests) {
public:
    TEST_METHOD_INITIALIZE(JsonObjectConverterTestsSetUp) {
        valueConverter_ = ref new JsonObjectConverter();
        sampleJson_ = JsonObject::Parse("{\"numericKey\":1,\"numericKeyCompound\":{\"value\":11},\"stringKey\":\"string\",\"stringKeyCompound\":{\"value\":{\"value\":\"compound\"}},\"boolKey\":true,\"boolKeyCompound\":{\"value\":false},\"nullKey\":null,\"nullKeyCompound\":{\"value\":null}}");
    }

    TEST_METHOD(JsonObjectConverterTestsConvertBySimpleName) {
        auto resultStr = static_cast<String^>(valueConverter_->Convert(sampleJson_, String::typeid, "stringKey",
            nullptr));
        auto resultNumeric = static_cast<double>(valueConverter_->Convert(sampleJson_, double::typeid, "numericKey",
            nullptr));
        auto resultBool = static_cast<bool>(valueConverter_->Convert(sampleJson_, bool::typeid, "boolKey", nullptr));
        auto resultNull = static_cast<String^>(valueConverter_->Convert(sampleJson_, String::typeid, "nullKey",
            nullptr));
        auto resultNumericAsStr = static_cast<String^>(valueConverter_->Convert(sampleJson_, String::typeid, "numericKey",
            nullptr));
        auto resultBoolAsStr = static_cast<String^>(valueConverter_->Convert(sampleJson_, String::typeid, "boolKey",
            nullptr));

        Assert::AreEqual("string", resultStr);
        Assert::AreEqual(1.0, resultNumeric);
        Assert::IsTrue(resultBool);
        Assert::IsNull(resultNull);
        Assert::AreEqual("1", resultNumericAsStr);
        Assert::AreEqual("true", resultBoolAsStr);
    }

    TEST_METHOD(JsonObjectConverterTestsConvertByCompoundName) {
        auto resultStr = static_cast<String^>(valueConverter_->Convert(sampleJson_, String::typeid,
            "stringKeyCompound.value.value", nullptr));
        auto resultBool = static_cast<bool>(valueConverter_->Convert(sampleJson_, bool::typeid, "boolKeyCompound.value",
            nullptr));
        auto resultNull = valueConverter_->Convert(sampleJson_, String::typeid, "nullKeyCompound.value",
            nullptr);
        auto resultNumeric = static_cast<double>(valueConverter_->Convert(sampleJson_, double::typeid,
            "numericKeyCompound.value", nullptr));

        Assert::AreEqual("compound", resultStr);
        Assert::IsFalse(resultBool);
        Assert::AreEqual(11.0, resultNumeric);
        Assert::IsNull(resultNull);
    }

    TEST_METHOD(JsonObjectConverterTestsConvertByNullName) {
        Assert::IsNull(valueConverter_->Convert(sampleJson_, String::typeid, nullptr, nullptr));
    }

    TEST_METHOD(JsonObjectConverterTestsConvertByEmptyName) {
        Assert::IsNull(valueConverter_->Convert(sampleJson_, String::typeid, nullptr, ""));
    }

    TEST_METHOD(JsonObjectConverterTestsConvertByMissingKey) {
        Assert::IsNull(valueConverter_->Convert(sampleJson_, String::typeid, "invalidKey", nullptr));
        Assert::IsNull(valueConverter_->Convert(sampleJson_, String::typeid, "boolKeyCompound.invalidKey", nullptr));
    }

    TEST_METHOD(JsonObjectConverterTestsConvertInvalidObjectType) {
        Assert::IsNull(valueConverter_->Convert("hello", String::typeid, "boolKey", nullptr));
    }

    TEST_METHOD(JsonObjectConverterTestsConvertInvalidConversion) {
        auto resultStr = static_cast<String^>(valueConverter_->Convert(sampleJson_, Object::typeid, "stringKey",
            nullptr));

        Assert::AreEqual("string", resultStr);
    }

private:
    IValueConverter^ valueConverter_;
    IJsonObject^ sampleJson_;
};
}
}
}
}