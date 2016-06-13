#ifndef VoiceMyNewsAppTests_utils_TestConstants_H_
#define VoiceMyNewsAppTests_utils_TestConstants_H_

#include <chrono>

namespace voicemynews {
namespace tests {
/**
 * This constant controls the maximum allowed number of milliseconds we are willing to wait for a test async operation
 * to finish.
 */
auto kTestAsyncMaximumTimeout = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(10000));
}
}

#endif // VoiceMyNewsAppTests_utils_TestConstants_H_
