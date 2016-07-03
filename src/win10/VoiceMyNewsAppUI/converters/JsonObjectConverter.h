#ifndef VoiceMyNewsAppUI_converters_JsonObjectConverter_H_
#define VoiceMyNewsAppUI_converters_JsonObjectConverter_H_

namespace voicemynews {
namespace app {
namespace win10 {
namespace converters {
using Platform::Object;
using Platform::String;
using Windows::UI::Xaml::Data::IValueConverter;
using Windows::UI::Xaml::Interop::TypeName;

/**
 * \brief Provides a mechanism for converting a json object to a displayable string.
 *
 * It does this by accepting an attribute name which will be extracted from the json object.
 */
[Windows::Foundation::Metadata::WebHostHiddenAttribute]
public ref class JsonObjectConverter sealed : IValueConverter {
public:
    virtual Object^ Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language);

    virtual Object^ ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language);
};
}
}
}
}

#endif /* VoiceMyNewsAppUI_converters_JsonObjectConverter_H_ */
