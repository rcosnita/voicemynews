#ifndef VoiceMyNewsCoreIntegrationTests_utils_TestIOUtils_H_
#define VoiceMyNewsCoreIntegrationTests_utils_TestIOUtils_H_

#include <string>

namespace voicemynews {
namespace tests {
namespace integration {
namespace utils {
/**
 * \class TestIOUtils
 * \brief This class provides I/O helper methods used within tests.
 */
class TestIOUtils {
public:
    /**
     * \brief This method load the specified fileName and returns its content.
     *
     * \param fileName the absolute / relative path of the file we want to load.
     *
     * \returns the content of the specified file.
     */
    std::wstring ReadFile(const std::string& fileName);
};
}
}
}
}

#endif // VoiceMyNewsCoreIntegrationTests_utils_TestIOUtils_H_