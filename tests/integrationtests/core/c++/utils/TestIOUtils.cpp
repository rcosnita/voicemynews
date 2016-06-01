#include "utils\TestIOUtils.h"

#include <fstream>

namespace voicemynews {
namespace tests {
namespace integration {
namespace utils {
std::wstring TestIOUtils::ReadFile(const std::string& fileName) {
    std::ifstream fileStream(fileName);
    std::wstring str((std::istreambuf_iterator<char>(fileStream)), 
                    std::istreambuf_iterator<char>());

    return str;
}
}
}
}
}