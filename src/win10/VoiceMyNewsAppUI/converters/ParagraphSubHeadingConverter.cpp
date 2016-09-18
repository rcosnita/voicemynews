#include "pch.h"
#include "converters/JsonObjectConverter.h"
#include "converters/ParagraphSubHeadingConverter.h"
#include "utils/Conversions.h"

#include <string>

using Platform::Object;
using Platform::String;
using Windows::UI::Xaml::Interop::TypeName;
using Windows::UI::Text::FontWeights;

using voicemynews::app::win10::utils::ConvertPlatformStrToStd;

const std::string kFontWeightParameter = "fontWeight";
const std::string kFontSizeParameter = "fontSize";
static Platform::String^ kSubHeadingPropertyName = "subheadingLevel";

static voicemynews::app::win10::converters::JsonObjectConverter^ s_jsonConverter =
    ref new voicemynews::app::win10::converters::JsonObjectConverter();

namespace voicemynews {
namespace app {
namespace win10 {
namespace converters {
Object^ ParagraphSubHeadingConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
    auto parameterStd = ConvertPlatformStrToStd(parameter->ToString());
    if (parameterStd.compare(kFontWeightParameter) == 0) {
       return ObtainFontWeight(value, language);
    }
    else if (parameterStd.compare(kFontSizeParameter) == 0) {
        return ObtainFontSize(value, language);
    }

    throw ref new Platform::NotImplementedException();
}

Object^ ParagraphSubHeadingConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter,
    String^ language)
{
    throw ref new Platform::NotImplementedException();
}

Windows::UI::Text::FontWeight ParagraphSubHeadingConverter::ObtainFontWeight(Object^ jsonDesc, String^ language)
{
    int headingLevel = -1;

    try {
        auto value = static_cast<double>(s_jsonConverter->Convert(jsonDesc, double::typeid, kSubHeadingPropertyName,
            language));
        headingLevel = static_cast<int>(value);
    } catch (Platform::Exception^) {
        return FontWeights::Normal;
    }

    return headingLevel <= 3 ? FontWeights::Bold : FontWeights::Normal;
}

double ParagraphSubHeadingConverter::ObtainFontSize(Object^ jsonDesc, String^ language)
{
    int headingLevel = -1;

    try {
        auto value = static_cast<double>(s_jsonConverter->Convert(jsonDesc, double::typeid, kSubHeadingPropertyName,
            language));
        headingLevel = static_cast<int>(value);
    } catch (Platform::Exception^) {
        return 20;
    }

    return headingLevel <= 3 ? 30 : 20;
}
}
}
}
}