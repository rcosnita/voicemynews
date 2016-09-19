#include "pch.h"
#include "CppUnitTest.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Platform::Object;
using Platform::String;

using voicemynews::app::win10::converters::ParagraphSubHeadingConverter;

using Windows::Data::Json::IJsonObject;
using Windows::Data::Json::JsonObject;
using Windows::Data::Json::JsonValue;
using Windows::UI::Text::FontWeight;
using Windows::UI::Text::FontWeights;
using Windows::UI::Xaml::Data::IValueConverter;
using Windows::UI::Xaml::Interop::TypeName;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace converters {
/**
 * \brief Provides the tests suite which ensures correct functionality of paragraph subheading converter.
 */
TEST_CLASS(ParagraphSubHeadingConverterTests)
{
public:
    TEST_METHOD_INITIALIZE(ParagraphSubHeadingConverterTestsInit)
    {
        valueConverter_ = ref new ParagraphSubHeadingConverter();
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsLevel1FontWeightOk)
    {
        TestPagraphSubHeadingFontWeightTemplate(1, FontWeights::Bold);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsLevel2FontWeightOk)
    {
        TestPagraphSubHeadingFontWeightTemplate(2, FontWeights::Bold);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsLevel3FontWeightOk)
    {
        TestPagraphSubHeadingFontWeightTemplate(3, FontWeights::Bold);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsLevel4FontWeightOk)
    {
        TestPagraphSubHeadingFontWeightTemplate(4, FontWeights::Normal);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsLevel5FontWeightOk)
    {
        TestPagraphSubHeadingFontWeightTemplate(5, FontWeights::Normal);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsLevel6FontWeightOk)
    {
        TestPagraphSubHeadingFontWeightTemplate(6, FontWeights::Normal);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsFontSizeLevel1Ok)
    {
        TestParagraphSubHeadingFontSizeTemplate(1, 30);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsFontSizeLevel2Ok)
    {
        TestParagraphSubHeadingFontSizeTemplate(2, 22);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsFontSizeLevel3Ok)
    {
        TestParagraphSubHeadingFontSizeTemplate(3, 20);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsFontSizeLevel4Ok)
    {
        TestParagraphSubHeadingFontSizeTemplate(4, 15);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsFontSizeLevel5Ok)
    {
        TestParagraphSubHeadingFontSizeTemplate(5, 15);
    }

    TEST_METHOD(ParagraphSubHeadingConverterTestsFontSizeLevel6Ok)
    {
        TestParagraphSubHeadingFontSizeTemplate(6, 15);
    }

private:
    /**
     * \brief Provides a template method which can test font weight retrieval logic and makes relevant asserts.
     */
    void TestPagraphSubHeadingFontWeightTemplate(int subheadingLevel, FontWeight expectedWeight)
    {
        auto obj = ref new JsonObject();
        obj->SetNamedValue("subheadingLevel", JsonValue::CreateNumberValue(subheadingLevel));

        auto result = safe_cast<FontWeight>(valueConverter_->Convert(obj, String::typeid, "fontWeight", "en_US"));
        Assert::AreEqual(static_cast<int>(expectedWeight.Weight), static_cast<int>(result.Weight));
    }

    /**
     * \brief Provides a template method which can test font size retrieval logic and makes relevant asserts.
     */
    void TestParagraphSubHeadingFontSizeTemplate(int subheadingLevel, double expectedSize)
    {
        auto obj = ref new JsonObject();
        obj->SetNamedValue("subheadingLevel", JsonValue::CreateNumberValue(subheadingLevel));

        auto result = static_cast<double>(valueConverter_->Convert(obj, String::typeid, "fontSize", "en_US"));
        Assert::AreEqual(expectedSize, result);
    }

private:
    IValueConverter^ valueConverter_;
};
}
}
}
}