#include "events/EventData.h"
#include "events/EventLoop.h"
#include "gtest/gtest.h"

#include <atomic>
#include <thread>

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace core {
namespace events {
using voicemynews::core::events::EventData;
using voicemynews::core::events::EventLoop;

class EventLoopTest : public ::testing::Test {
public:
    EventLoopTest() = default;

    virtual ~EventLoopTest() = default;

    virtual void SetUp() {
        eventLoop = new EventLoop();
    }

    virtual void TearDown() { }

public:
    EventLoop* eventLoop;
};

TEST_F(EventLoopTest, TestEmitUnregisteredEvent) {
    eventLoop->Emit("customEvent", std::make_shared<EventData<void>>());

    eventLoop->ProcessEvents();
}

TEST_F(EventLoopTest, TestEmitRegisteredEventOk) {
    const std::string kEventName = "event";
    bool invoked = false;

    eventLoop->On<void>(kEventName, [&invoked](std::shared_ptr<EventData<void>> evtData) {
        invoked = true;
    });

    eventLoop->Emit(kEventName, std::make_shared<EventData<void>>());
    eventLoop->ProcessEvents();

    EXPECT_TRUE(invoked);
}

TEST_F(EventLoopTest, TestProcessEventsOk) {
    const int kMaxThreads = 100;
    const std::string kEvtName = "customEvent";

    std::atomic_int listenerInvokedCounter = 0;

    eventLoop->On<void*>(kEvtName, [&listenerInvokedCounter](std::shared_ptr<EventData<void*>> evtData) {
        listenerInvokedCounter++;
    });

    auto processEvents = std::thread([&kMaxThreads, &listenerInvokedCounter, this]() {
        while (listenerInvokedCounter < kMaxThreads) {
            eventLoop->ProcessEvents();
        }
    });

    std::thread threads[kMaxThreads];

    for (auto idx = 0; idx < kMaxThreads; idx++) {
        threads[idx] = std::thread([&kEvtName, this]() {
            eventLoop->Emit(kEvtName, std::shared_ptr<EventData<void*>>());
        });
    }

    for (auto idx = 0; idx < kMaxThreads; idx++) {
        threads[idx].join();
    }

    processEvents.join();

    EXPECT_EQ(kMaxThreads, listenerInvokedCounter);
}
}
}
}
}
}