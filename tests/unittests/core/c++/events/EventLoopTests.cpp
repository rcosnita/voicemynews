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

TEST_F(EventLoopTest, TestProcessEventsEmptyQueue) {
    eventLoop->ProcessEvents();
}

TEST_F(EventLoopTest, TestEmitRegisteredEventOk) {
    const std::string kEventName = "event";
    bool invoked = false;

    std::function<void(std::shared_ptr<EventData<void>>)> fn = [&invoked](std::shared_ptr<EventData<void>> evtData) {
        invoked = true;
    };

    eventLoop->On<void>(kEventName, fn);

    eventLoop->Emit(kEventName, std::make_shared<EventData<void>>());
    eventLoop->ProcessEvents();

    EXPECT_TRUE(invoked);
}

TEST_F(EventLoopTest, TestOnOffOk) {
    const std::string kEventName = "myEvent";

    EXPECT_EQ(0, eventLoop->GetListenersCount("not registered event"));
    EXPECT_EQ(0, eventLoop->GetListenersCount("not registered event 2"));
    EXPECT_EQ(0, eventLoop->GetListenersCount(kEventName));

    std::function<void(std::shared_ptr<EventData<void>>)> fn1 = [](std::shared_ptr<EventData<void>>) { };
    std::function<void(std::shared_ptr<EventData<void>>)> fn2 = [](std::shared_ptr<EventData<void>>) { };

    eventLoop->On<void>(kEventName, fn1);
    EXPECT_EQ(1, eventLoop->GetListenersCount(kEventName));

    eventLoop->On<void>(kEventName, fn2);
    EXPECT_EQ(2, eventLoop->GetListenersCount(kEventName));

    eventLoop->Off(kEventName, &fn1);
    EXPECT_EQ(1, eventLoop->GetListenersCount(kEventName));

    eventLoop->Off(kEventName, &fn2);
    EXPECT_EQ(0, eventLoop->GetListenersCount(kEventName));
}

TEST_F(EventLoopTest, TestOffMethodNotRegisteredForExistingEvent) {
    const std::string kEvtName = "sample event";
    std::function<void(std::shared_ptr<EventData<void>>)> fn = [](std::shared_ptr<EventData<void>>) { };
    std::function<void(std::shared_ptr<EventData<void>>)> fn2 = [](std::shared_ptr<EventData<void>>) {};

    eventLoop->On(kEvtName, fn2);
    EXPECT_EQ(1, eventLoop->GetListenersCount(kEvtName));

    eventLoop->Off(kEvtName, &fn);
    EXPECT_EQ(1, eventLoop->GetListenersCount(kEvtName));

    eventLoop->Off(kEvtName, &fn);
    EXPECT_EQ(1, eventLoop->GetListenersCount(kEvtName));
}

TEST_F(EventLoopTest, TestProcessEventsOk) {
    const int kMaxThreads = 100;
    const std::string kEvtName = "customEvent";

    std::atomic_int listenerInvokedCounter = 0;

    std::function<void(std::shared_ptr<EventData<void*>>)> fn = [&listenerInvokedCounter](std::shared_ptr<EventData<void*>> evtData) {
        listenerInvokedCounter++;
    };

    eventLoop->On<void*>(kEvtName, fn);

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