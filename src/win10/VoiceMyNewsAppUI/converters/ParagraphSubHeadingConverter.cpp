#include "pch.h"
#include "converters/JsonObjectConverter.h"
#include "converters/ParagraphSubHeadingConverter.h"
#include "utils/Conversions.h"

#include <math.h>
#include <string>

using Platform::Object;
using Platform::String;
using Windows::UI::Xaml::Interop::TypeName;
using Windows::UI::Text::FontWeights;

using voicemynews::app::win10::utils::ConvertPlatformStrToStd;

/**
 * Holds the value of the parameter passed to converter which tells the converter to calculate font weight.
 */
const std::string kFontWeightParameter = "fontWeight";

/**
 * Holds the value of the parameter passed to converter which tells the converter to calculate font size.
 */
const std::string kFontSizeParameter = "fontSize";

/// TODO [rcosnita] Instead of using a hardcoded value use a platform api to obtain it.
const double kDefaultFontSize = 15;

/**
 * Holds the subheading level property name which is available for json descriptor.
 */
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
    int headingLevel = GetSubHeadingLevel(jsonDesc);
    return headingLevel <= 3 && headingLevel > 0 ? FontWeights::Bold : FontWeights::Normal;
}

double ParagraphSubHeadingConverter::ObtainFontSize(Object^ jsonDesc, String^ language)
{
    int headingLevel = GetSubHeadingLevel(jsonDesc);
    double fontSize = headingLevel <= 3 && headingLevel > 0 ? kDefaultFontSize / headingLevel + kDefaultFontSize : kDefaultFontSize;
    return std::floor(fontSize);
}

int ParagraphSubHeadingConverter::GetSubHeadingLevel(Object^ jsonDesc)
{
    auto value = static_cast<double>(s_jsonConverter->Convert(jsonDesc, double::typeid, kSubHeadingPropertyName,
            nullptr));
    return static_cast<int>(value);
}
}
}
}
}