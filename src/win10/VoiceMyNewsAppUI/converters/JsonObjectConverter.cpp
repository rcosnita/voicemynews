#include "pch.h"
#include "utils/Conversions.h"
#include "JsonObjectConverter.h"

#include <string>

using Platform::Object;
using Platform::String;
using voicemynews::app::win10::utils::ConvertPlatformStrToStd;
using Windows::Data::Json::IJsonObject;
using Windows::Data::Json::IJsonValue;
using Windows::Data::Json::JsonValueType;
using Windows::UI::Xaml::Interop::TypeName;

/**
 * \brief Provides a mechanism for converting the given value to a type compatible with targettype.
 * In case no conversion succeeds we will return the value as is.
 */
template<typename T>
static Object^ ConvertTo(T value, TypeName targetType) {
    if (TypeName(String::typeid).Name->Equals(targetType.Name)) {
        String^ result = "" + value;
        return result;
    }

    return value;
}

/**
 * \brief In case parameter name contains . in it, the method tries to go recursively till requested level.
 * If the given parameter does not contain name then the master object is returned.
 */
static IJsonObject^ GetFromCompoundParameter(IJsonObject^ master, String^& parameter) {
    std::wstring paramName = parameter->Data();
    size_t dotPos = -1;

    while ((dotPos = paramName.find('.')) > 0) {
        String^ currKey = ref new String(paramName.substr(0, dotPos).data());
        paramName = paramName.substr(dotPos + 1);

        try {
            master = master->GetNamedObject(currKey);
        } catch (...) {
            return nullptr;
        }
    }

    parameter = ref new String(paramName.data());
    return master;
}

namespace voicemynews {
namespace app {
namespace win10 {
namespace converters {
Object^ JsonObjectConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language) {
    if (parameter == nullptr) {
        return nullptr;
    }

    auto parameterStr = parameter->ToString();
    IJsonObject^ jsonObj = nullptr;

    try {
        jsonObj = safe_cast<IJsonObject^>(value);
    } catch (...) {
        return nullptr;
    }

    jsonObj = GetFromCompoundParameter(jsonObj, parameterStr);

    if (jsonObj == nullptr) {
        return nullptr;
    }

    IJsonValue^ jsonValue = nullptr;

    try {
        jsonValue = jsonObj->GetNamedValue(parameterStr);
    } catch (...) {
        return nullptr;
    }

    switch (jsonValue->ValueType) {
    case JsonValueType::Number:
        return ConvertTo(jsonValue->GetNumber(), targetType);

    case JsonValueType::String:
        return ConvertTo(jsonValue->GetString(), targetType);

    case JsonValueType::Boolean:
        return ConvertTo(jsonValue->GetBoolean(), targetType);

    case JsonValueType::Null:
        return nullptr;
    }

    throw ref new Platform::Exception(-1, "Not implemented yet ...");
}

Object^ JsonObjectConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language) {
    throw ref new Platform::NotImplementedException();
}

}
}
}
}