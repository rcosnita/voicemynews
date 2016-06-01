#ifndef VoiceMyNewsCore_IO_FS_Exceptions_FileNotFoundException_H_
#define VoiceMyNewsCore_IO_FS_Exceptions_FileNotFoundException_H_

#include <exception>
#include <string>

namespace voicemynews {
namespace core {
namespace io {
namespace fs {
namespace exceptions {
/**
 * \class FileNotFoundException
 * \brief This class provides an exception which is raised whenever a file is not found.
 */
class FileNotFoundException : std::exception {
public:
    FileNotFoundException(std::string errMsg, std::string fileName);
    virtual ~FileNotFoundException() = default;

    /**
     * \brief The current implementation builds a friendly error message for the current instance.
     */
    virtual const char* what() const;

    virtual const std::string FileName() const;
private:
    std::string errMsg_;
    std::string fileName_;
};
}
}
}
}
}

#endif // VoiceMyNewsCore_IO_FS_Exceptions_FileNotFoundException_H_
