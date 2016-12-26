#ifndef app_VoiceMyNews_android_utils_DataWrapper_H
#define app_VoiceMyNews_android_utils_DataWrapper_H

namespace voicemynews {
namespace app {
namespace android {
namespace utils {
/**
 * \brief Provides a simple wrapper which can be used to carry any type of smart pointers.
 *
 * It is especially helpful when we carry smart pointers between native C++ and V8 js engine.
 */
template<typename T>
class DataWrapper
{
private:
    T data_;

public:
    DataWrapper(T& data) : data_(data) { }
    DataWrapper(T&& data) : data_(data) { }

    virtual ~DataWrapper()
    {
        data_.reset();
    }

    T Data()
    {
        return data_;
    }
};
}
}
}
}

#endif /* app_VoiceMyNews_android_utils_DataWrapper_H */