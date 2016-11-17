#include "io/fs/FileUtilsPlatform.h"
static AAssetManager* assetManager_ = nullptr;

namespace voicemynews {
namespace core {
namespace io {
namespace fs {
void FileUtilsPlatform::Initialize(AAssetManager *assetManager)
{
    assetManager_ = assetManager;
}

std::wstring FileUtilsPlatform::ReadFilePlatform(const std::string& filename)
{
    AAsset* asset = AAssetManager_open(assetManager_, filename.c_str(), AASSET_MODE_UNKNOWN);
    long size = AAsset_getLength(asset);
    char* buffer = (char*) calloc(size + 1, sizeof(char));
    AAsset_read (asset, buffer, size);
    AAsset_close(asset);

    std::string narrow(buffer);
    return std::wstring(narrow.begin(), narrow.end());
}
}
}
}
}