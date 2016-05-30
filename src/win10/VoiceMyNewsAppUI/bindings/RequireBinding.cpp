#include "pch.h"

#include "RequireBinding.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
RequireBinding::RequireBinding()
    : require_(std::make_shared<FileUtils>()) {
}

String^ RequireBinding::Load(String^ fileName) {
    std::wstring fileNameW(fileName->Data());
    std::wstring moduleSource = require_.Load(std::string(fileNameW.begin(), fileNameW.end()));

    return ref new String(moduleSource.c_str());
}
}
}
}
}