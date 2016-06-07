#include "pch.h"
#include "Conversions.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace utils {
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
}
}
}
}