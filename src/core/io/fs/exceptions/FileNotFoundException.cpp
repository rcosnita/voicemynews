#include "FileNotFoundException.h"

namespace voicemynews {
namespace core {
namespace io {
namespace fs {
namespace exceptions {
FileNotFoundException::FileNotFoundException(std::string errMsg, std::string fileName)
    : std::exception(),
    errMsg_(errMsg),
    fileName_(fileName) {

}

const char* FileNotFoundException::what() const _NOEXCEPT { return errMsg_.c_str(); }

const std::string FileNotFoundException::FileName() const { return fileName_; }

}
}
}
}
}