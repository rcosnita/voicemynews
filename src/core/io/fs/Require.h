#ifndef VoiceMyNewsCore_IO_FS_Require_H_
#define VoiceMyNewsCore_IO_FS_Require_H_

#include <memory>
#include <string>

#include "FileUtils.h"

namespace voicemynews {
namespace core {
namespace io {
namespace fs {
/**
 * \class Require
 * \brief This class provides the foundation for implementing requirejs common module support.
 *
 * The class provides a fully compliant implementation with nodejs require style. We do this by relying only on C++
 * std library in order to be able to reuse it cross platform.
 */
class Require {
public:
    Require(std::shared_ptr<FileUtils>);
    virtual ~Require() = default;

    /**
     * \brief This method tries to wrap the given filename content as a commonjs module.
     *
     * This method uses string processing in order to load the given filename content and decorate it with commonjs
     * annonymous function decoration. It is designed so that requirejs can easily embed the output of this function
     * and eventually cache it if necessary.
     *
     * \param fileName the relative / absolute path of the file we want to load.
     *
     * \return The file content wrapped within anonymous commonjs function.
     */
    std::wstring Load(const std::string& fileName);

    /**
     * \brief This method loads the content from the specified file and returns its content.
     *
     * \param fileName the relative / absolute path of the file we want to load.
     * \return The raw string content of the specified file.
     */
    std::wstring LoadRaw(const std::string& fileName);

private:
    /**
     * \brief This method decorate the given module script with anonymous require function.
     *
     * \param moduleScript The script which potentially uses module.exports.
     * \return Nothing because the decoration is done in place in moduleScript.
     */
    void DecorateWithAnonymousFunction(std::wstring& moduleScript);

private:
    const std::wstring kRequireAnonymousFunctionHeader = L"((module, exports) => {";
    const std::wstring kRequireAnonymousFunctionFooter = L"return module;})(module, exports);";

    std::shared_ptr<FileUtils> fileUtils_;
};
}
}
}
}

#endif // VoiceMyNewsCore_IO_FS_Require_H_