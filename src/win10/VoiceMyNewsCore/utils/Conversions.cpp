#include "Conversions.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace utils {
using Platform::Collections::Map;
using Platform::Collections::Vector;

    std::string ConvertPlatformStrToStd(Platform::String^ str) {
        std::wstring strW = str->Data();

        return std::string(strW.begin(), strW.end());
    }

    Platform::String^ ConvertStdStrToPlatform(std::string str) {
        std::wstring strW(str.begin(), str.end());

        return ref new Platform::String(strW.c_str());
    }

    Platform::String^ ConvertStdStrToPlatform(std::wstring str) {
        return ref new Platform::String(str.c_str());
    }

    IMap<Platform::String^, Platform::String^>^ ConvertStdMapToPlatform(std::map<std::string, std::string> stdMap) {
        auto result = ref new Map<Platform::String^, Platform::String^>();

        for (auto entry = stdMap.begin(); entry != stdMap.end(); entry++) {
            auto key = ConvertStdStrToPlatform(entry->first);
            auto value = ConvertStdStrToPlatform(entry->second);
            result->Insert(key, value);
        }

        return result;
    }

    IVector<IJsonObject^>^ ConvertJsonArrayToVector(JsonArray% jsonArray) {
        auto result = ref new Vector<IJsonObject^>();

        for (auto first = jsonArray.First(); first->HasCurrent; first->MoveNext()) {
            result->Append(first->Current->GetObject());
        }

        return result;
    }
}
}
}
}