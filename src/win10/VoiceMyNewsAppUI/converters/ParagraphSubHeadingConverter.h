#ifndef VoiceMyNewsAppUI_converters_ParagraphFontWeightConverter_H_
#define VoiceMyNewsAppUI_converters_ParagraphFontWeightConverter_H_

namespace voicemynews {
namespace app {
namespace win10 {
namespace converters {
/**
 * \brief Provides a converter which can determine the font weight for the given value object.
 *
 * It relies on voicemynews::app::win10::converters::JsonObjectConverter for extracting the attribute name given as
 * parameter and taking decisions based on it.
 */
[Windows::Foundation::Metadata::WebHostHiddenAttribute]
public ref class ParagraphSubHeadingConverter sealed : public Windows::UI::Xaml::Data::IValueConverter
{
using Object = Platform::Object;
using String = Platform::String;
using TypeName = Windows::UI::Xaml::Interop::TypeName;

public:
    virtual Object^ Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language);

    virtual Object^ ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language);

private:
    /**
     * \brief Provides the logic for obtaining the font weight from the given json descriptor.
     */
    Windows::UI::Text::FontWeight ObtainFontWeight(Object^ jsonDesc, String^ language);

    /**
    * \brief Provides the logic for obtaining the font size from the given json descriptor.
    */
    double ObtainFontSize(Object^ jsonDesc, String^ language);

    /**
     * \brief Provides the logic for obtaining the heading level from the given json descriptor.
     *
     * If the heading level is not found it returns -1.
     */
    int GetSubHeadingLevel(Object^ jsonDesc);
};
}
}
}
}

#endif /* VoiceMyNewsAppUI_converters_ParagraphFontWeightConverter_H_ */
