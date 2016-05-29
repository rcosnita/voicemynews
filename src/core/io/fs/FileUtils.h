#ifndef VoiceMyNewsCore_IO_FS_FileUtils_H_
#define VoiceMyNewsCore_IO_FS_FileUtils_H_

#include <string>

namespace voicemynews {
namespace core {
namespace io {
namespace fs {
/**
 * \class FileUtils
 * \brief This class provides a cross platform implementation for file operations.
 *
 * This class usually won't be binded directly into js. Instead, higher levels implementation
 * will be made available to js.
 */
class FileUtils {
public:
    /**
     * \brief This method reads the file content and returns its content.
     *
     * \param fileName The file absolute / relative path we want to read.
     * \return the content of the file.
     *
     * \throws voicemynews::core::io::fs::exceptions::FileNotFoundException in case the given fileName is not found
     *  or cannot be opened.
     */
    std::wstring ReadFile(const std::string& fileName);
};

}
}
}
}

#endif // VoiceMyNewsCore_IO_FS_FileUtils_H_