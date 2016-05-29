#include "analytics/Analytics.h"
#include "gtest/gtest.h"

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace core {
class AnalyticsTest : public ::testing::Test {
public:
    AnalyticsTest() = default;

    virtual ~AnalyticsTest() = default;

    virtual void SetUp() { }

    virtual void TearDown() { }
};

TEST_F(AnalyticsTest, AnalyticsSayHelloOk) {
    std::string msg = "Hello world !!!";
    auto output = voicemynews::core::analytics::SayHello(msg);

    EXPECT_EQ(msg, output);
}
}
}
}
}