#include "analytics/Analytics.h"
#include "events/EventNames.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <memory>

using voicemynews::core::analytics::Analytics;
using voicemynews::core::analytics::AnalyticsEvent;
using voicemynews::core::analytics::WebBrowser;

using voicemynews::core::events::kAnalyticsLogEvent;

using ::testing::_;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace core {
class MockWebBrowser : public WebBrowser
{
public:
    MOCK_METHOD1(LoadContent, void(std::string));

    MOCK_METHOD2(SendEvent, void(std::string, std::string));
};

class AnalyticsTest : public ::testing::Test
{
public:
    MockWebBrowser* webBrowser;
    Analytics* analyticsInstance = nullptr;

    AnalyticsTest() = default;

    virtual ~AnalyticsTest() = default;

    virtual void SetUp()
    {
        webBrowser = new MockWebBrowser();
        analyticsInstance = new Analytics(std::shared_ptr<WebBrowser>(webBrowser));
    }

    virtual void TearDown()
    {
        delete analyticsInstance;
    }
};

class AnalyticsLogEventTest : public ::testing::Test
{
public:
    AnalyticsLogEventTest() = default;
    virtual ~AnalyticsLogEventTest() = default;
};

TEST_F(AnalyticsTest, AnalyticsLogEventOk)
{
    AnalyticsEvent evt("category", "action", "label", 20);
    auto evtData = evt.ToJson();
    EXPECT_CALL(*webBrowser, SendEvent(kAnalyticsLogEvent, evtData));

    analyticsInstance->LogEvent(evt);
}

TEST_F(AnalyticsLogEventTest, ToJsonOk)
{
    AnalyticsEvent evt("category", "action", "label", 20);
    std::string expectedJson = "{\"eventCategory\": \"category\",\"eventAction\": \"action\",\"eventLabel\": \"label\",\"eventValue\": 20,\"custom-eventName\": \"js:analytics:log:event\"}";
    auto json = evt.ToJson();

    EXPECT_EQ(expectedJson, json);
}
}
}
}
}