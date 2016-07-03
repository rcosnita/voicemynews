#include "pch.h"
#include "JsonObjectConverter.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace converters {
using Windows::Data::Json::IJsonObject;

Object^ JsonObjectConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language) {
    auto jsonObj = static_cast<IJsonObject^>(value);

    return jsonObj->GetNamedValue(parameter->ToString())->GetString();
}

Object^ JsonObjectConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language) {
    throw ref new Platform::NotImplementedException();
}

}
}
}
}