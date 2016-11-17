#include "Require.h"

#include <fstream>

namespace voicemynews {
namespace core {
namespace io {
namespace fs {
Require::Require(std::shared_ptr<FileUtils> fileUtils)
    : fileUtils_(fileUtils) {

}

std::wstring Require::Load(const std::string& fileName) {
    auto content = fileUtils_->ReadFilePlatform(fileName);
    DecorateWithAnonymousFunction(content);

    return content;
}

std::wstring Require::LoadRaw(const std::string& fileName) {
    return fileUtils_->ReadFilePlatform(fileName);
}

void Require::DecorateWithAnonymousFunction(std::wstring& moduleScript) {
    moduleScript.insert(0, kRequireAnonymousFunctionHeader);
    moduleScript.insert(kRequireAnonymousFunctionHeader.length(), L"\n");
    moduleScript.append(L"\n");
    moduleScript.append(kRequireAnonymousFunctionFooter);
}
}
}
}
}