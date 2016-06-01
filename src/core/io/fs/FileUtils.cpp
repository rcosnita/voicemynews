#include <fstream>

#include "FileUtils.h"
#include "exceptions/FileNotFoundException.h"

namespace voicemynews {
namespace core {
namespace io {
namespace fs {
using voicemynews::core::io::fs::exceptions::FileNotFoundException;

std::wstring FileUtils::ReadFile(const std::string& fileName) {
    std::ifstream fileStream(fileName);
    if (fileStream.fail()) {
        throw FileNotFoundException("File " + fileName + " not found.", fileName);
    }

    std::wstring str((std::istreambuf_iterator<char>(fileStream)),
        std::istreambuf_iterator<char>());

    return str;
}
}
}
}
}