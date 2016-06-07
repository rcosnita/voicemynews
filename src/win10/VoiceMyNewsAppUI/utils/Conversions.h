#ifndef VoiceMyNewsAppUI_utils_conversions_H_
#define VoiceMyNewsAppUI_utils_conversions_H_

#include <string>
#include <map>

namespace voicemynews {
namespace app {
namespace win10 {
namespace utils {
    /**
     * \brief This method provides a helper for converting platform string to standar string.
     *
     * \param str The platform wide string we want to convert to standard string.
     */
    std::string ConvertPlatformStrToStd(Platform::String^ str);

    /**
     * \brief This method provides a helper for converting a standard string to a Platform wide string.
     *
     * \param str standard string which we want to convert.
     */
    Platform::String^ ConvertStdStrToPlatform(std::string str);

    /**
     * \brief This method provides a helper for converting a standard wide string to a Platform wide string.
     *
     * \param str standard string which we want to convert.
     */
    Platform::String^ ConvertStdStrToPlatform(std::wstring str);

    /**
     * \brief This method provides a helper for converting a platform specific map with String key and value
     * into a standard map.
     *
     * \param platformMap the platform specific map.
     * \returns the standard map obtained from platformMap.
     */
    template<typename T>
    std::map<std::string, std::string> ConvertPlatformMapToStd(T platformMap) {
        std::map<std::string, std::string> mapStd;

        for (auto entry = platformMap->First(); entry->HasCurrent; entry->MoveNext()) {
            std::wstring keyW = entry->Current->Key->Data();
            std::wstring valueW = entry->Current->Value->Data();
            mapStd[std::string(keyW.begin(), keyW.end())] = std::string(valueW.begin(), valueW.end());
        }

        return mapStd;
    }
}
}
}
}

#endif // VoiceMyNewsAppUI_utils_conversions_H_