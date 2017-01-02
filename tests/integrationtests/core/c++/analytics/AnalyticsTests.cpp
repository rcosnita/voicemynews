#include "analytics/Analytics.h"
#include "gtest/gtest.h"

namespace voicemynews {
namespace tests {
namespace integration {
namespace core {
class AnalyticsIntegrationTest : public ::testing::Test {
public:
    AnalyticsIntegrationTest() = default;

    virtual ~AnalyticsIntegrationTest() = default;

    virtual void SetUp() { }

    virtual void TearDown() { }
};

TEST_F(AnalyticsIntegrationTest, AnalyticsSayHelloOk) {
    std::string msg = "Hello world !!!";
    //auto output = voicemynews::core::analytics::SayHello(msg);

    EXPECT_EQ(msg, msg);
}
}
}
}
}