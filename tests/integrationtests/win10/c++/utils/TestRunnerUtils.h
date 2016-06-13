#ifndef VoiceMyNewsAppTests_utils_TestRunnerUtils_H_
#define VoiceMyNewsAppTests_utils_TestRunnerUtils_H_

#include <functional>

namespace voicemynews {
namespace tests {
namespace utils {
/**
 * \brief This method provides a helper for executing tests which have an asynchronous phase which must end before test logic
 * can be executed.
 *
 * \param asyncLogic the function which encapsulates the test asynchronous logic. Each async logic method receives a done function
 * which must be invoked in order to signal the runner to move to the synchronous part. This is usually the last thing the async
 * logic will do.
 * \param testLogic the function which encapsulates test synchronous logic (usually asserts).
 */
template<typename T>
void RunTestAsync(std::function<void(std::function<void(T)>)> asyncLogic, std::function<void(T)> testLogic) {
    std::mutex syncObj;
    std::condition_variable cv;
    bool asyncLogicExecuted = false;

    T asyncData;
    asyncLogic([&syncObj, &asyncLogicExecuted, &cv, &asyncData](T& data) {
        std::lock_guard<std::mutex> lock(syncObj);

        asyncData = data;
        asyncLogicExecuted = true;
        cv.notify_one();
    });

    {
        std::unique_lock<std::mutex> lock(syncObj);

        cv.wait_for(lock, kTestAsyncMaximumTimeout,
            [&asyncLogicExecuted]() { return asyncLogicExecuted; });
    }

    testLogic(asyncData);
}
}
}
}

#endif // VoiceMyNewsAppTests_utils_TestRunnerUtils_H_
