#ifndef VoiceMyNewsAndroid_IO_FS_FileUtils_H_
#define VoiceMyNewsAndroid_IO_FS_FileUtils_H_

#include "io/fs/FileUtils.h"

#include <android/asset_manager.h>

namespace voicemynews {
namespace core {
namespace io {
namespace fs {
class FileUtilsPlatform : public FileUtils
{
public:
    /**
     * \brief Provides the platform specific read file implementation which reliest on AAssetManager api.
     */
    virtual std::wstring ReadFilePlatform(const std::string& filename) override;

public:
    /**
     * \brief Provides an injection method for singleton android platform specific classes.
     *
     * Developer must invoke this method only once when the platform starts.
     */
    static void Initialize(AAssetManager* assetManager);

};
}
}
}
}

#endif /* VoiceMyNewsAndroid_IO_FS_FileUtils_H_ */