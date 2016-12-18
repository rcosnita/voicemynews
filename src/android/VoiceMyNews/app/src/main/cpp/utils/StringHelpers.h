#ifndef app_VoiceMyNews_android_utils_StringHelpers_H
#define app_VoiceMyNews_android_utils_StringHelpers_H

#include <string>
#include <sstream>

namespace voicemynews {
namespace app {
namespace android {
namespace utils {
/**
 * \brief This class provides several helper string methods which are not built into standard library.
 */
class StringHelpers
{
public:
    /**
     * \brief Provides a generic to_string conversion method similar to the one std library would offer.
     * \param value The value we want to convert to string.
     * \returns the newly obtained string value.
     */
    template <typename T>
    static std::string to_string(T value)
    {
        std::ostringstream os ;
        os << value ;
        return os.str() ;
    }
};
}
}
}
}

#endif /* app_VoiceMyNews_android_utils_StringHelpers_H */