#include "pch.h"

#include "CppUnitTest.h"
#include "network/HttpClientInterface.h"
#include "utils/TestConstants.h"
#include "utils/TestRunnerUtils.h"

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
using voicemynews::tests::utils::RunTestAsync;

TEST_CLASS(HttpClientTest) {
public:
    TEST_METHOD_INITIALIZE(Setup) {
        httpClient_ = HttpClientInterface::GetHttpClientInstance();
    }

    TEST_METHOD_CLEANUP(TearDown) {
    }

    TEST_METHOD(HttpClientIntegrationTestGetStringOk) {
        RunTestAsync<std::shared_ptr<HttpResponseData<std::string>>>(
            [this](std::function<void(std::shared_ptr<HttpResponseData<std::string>>)> done) {
            httpClient_->Get("http://www.google.ro", headers_, queryParams_,
                [done](std::shared_ptr<HttpResponseData<std::string>> response) {
                done(response);
            });
        }, [](std::shared_ptr<HttpResponseData<std::string>> responseData) {
            Assert::IsTrue(static_cast<bool>(responseData));
            Assert::AreEqual(200, responseData->GetStatusCode());
            Assert::AreEqual(std::string("OK"), responseData->GetReason());

            auto responseHeaders = responseData->GetHeaders();

            Assert::IsTrue(responseHeaders["Content-Type"].find(std::string("text/plain;")) > 0);
            Assert::IsTrue(responseData->GetContent().length() > 0);
        });
    }

    TEST_METHOD(HttpClientIntegrationTestGetStringDomainNotFound) {
        RunTestAsync<std::shared_ptr<HttpResponseData<std::string>>>(
            [this](std::function<void(std::shared_ptr<HttpResponseData<std::string>>)> done) {
            httpClient_->Get("http://www.googlexassaddsa.ro/not/found/for/sure", headers_, queryParams_,
                [done](std::shared_ptr<HttpResponseData<std::string>> response) {
                done(response);
            });
        }, [](std::shared_ptr<HttpResponseData<std::string>> responseData) {
            Assert::IsTrue(static_cast<bool>(responseData));
            Assert::AreEqual(404, responseData->GetStatusCode());
            Assert::AreEqual(std::string("Not Found"), responseData->GetReason());

            auto responseHeaders = responseData->GetHeaders();
            Assert::IsTrue(responseHeaders.begin() == responseHeaders.end());
        });
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