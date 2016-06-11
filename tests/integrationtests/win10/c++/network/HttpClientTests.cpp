#include "pch.h"

#include "CppUnitTest.h"
#include "network/HttpClientInterface.h"
#include "utils/TestConstants.h"

#include <chrono>
#include <map>
#include <memory>
#include <mutex>

namespace voicemynews {
namespace tests {
namespace integrationtesting {
namespace core {
namespace network {
using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Platform::String;
using voicemynews::core::network::HttpClientInterface;
using voicemynews::core::network::HttpClientResponseStringCallback;
using voicemynews::core::network::HttpResponseData;
using voicemynews::tests::kTestAsyncMaximumTimeout;

TEST_CLASS(HttpClientTest) {
public:
    TEST_METHOD_INITIALIZE(Setup) {
        httpClient_ = HttpClientInterface::GetHttpClientInstance();
    }

    TEST_METHOD_CLEANUP(TearDown) {
    }

    TEST_METHOD(HttpClientIntegrationTestGetStringOk) {
        std::mutex syncObj;
        std::condition_variable cv;
        bool asyncLogicExecuted = false;
        std::shared_ptr<HttpResponseData<std::string>> responseData;

        httpClient_->Get("http://www.google.ro", headers_, queryParams_,
            [&syncObj, &asyncLogicExecuted, &responseData, &cv](std::shared_ptr<HttpResponseData<std::string>> response) {
            std::lock_guard<std::mutex> lock(syncObj);

            responseData = response;
            asyncLogicExecuted = true;

            cv.notify_one();
        });

        {
            std::unique_lock<std::mutex> lock(syncObj);

            cv.wait_for(lock, kTestAsyncMaximumTimeout,
                [&asyncLogicExecuted]() { return asyncLogicExecuted; });
        }

        Assert::IsTrue(static_cast<bool>(responseData));
        Assert::AreEqual(200, responseData->GetStatusCode());
        Assert::AreEqual(std::string("OK"), responseData->GetReason());
    }
private:
    std::map<std::string, std::string> headers_;
    std::shared_ptr<HttpClientInterface> httpClient_;
    std::map<std::string, std::string> queryParams_;
};
}
}
}
}
}