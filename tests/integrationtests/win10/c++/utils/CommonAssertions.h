#ifndef VoiceMyNewsAppTests_win10_cpp_utils_CommonAssertions_H_
#define VoiceMyNewsAppTests_win10_cpp_utils_CommonAssertions_H_

#include <jsrt.h>

namespace voicemynews {
namespace tests {
namespace app {
namespace win10 {
namespace utils {
namespace assertions {
    /**
    * \brief This method asserts that given result code is not a js error code.
    */
    void AssertNoJsError(const JsErrorCode jsErrorCode);
}
}
}
}
}
}

#endif //VoiceMyNewsAppTests_win10_cpp_utils_CommonAssertions_H_
