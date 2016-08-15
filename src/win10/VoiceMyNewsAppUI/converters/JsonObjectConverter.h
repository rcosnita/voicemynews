#ifndef VoiceMyNewsAppUI_converters_JsonObjectConverter_H_
#define VoiceMyNewsAppUI_converters_JsonObjectConverter_H_

namespace voicemynews {
namespace app {
namespace win10 {
namespace converters {
/**
 * \brief Provides a mechanism for converting a json object to a displayable string.
 *
 * It does this by accepting an attribute name which will be extracted from the json object.
 */
[Windows::Foundation::Metadata::WebHostHiddenAttribute]
public ref class JsonObjectConverter sealed : public Windows::UI::Xaml::Data::IValueConverter {
using Object = Platform::Object;
using String = Platform::String;
using TypeName = Windows::UI::Xaml::Interop::TypeName;

public:
    virtual Object^ Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language);

    virtual Object^ ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language);
};
}
}
}
}

#endif /* VoiceMyNewsAppUI_converters_JsonObjectConverter_H_ */
