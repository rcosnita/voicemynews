#include "EventLoop.h"

namespace voicemynews {
namespace core {
namespace events {
const std::chrono::duration<float> EventLoop::kPendingTimeout = std::chrono::milliseconds(1);

void EventLoop::ProcessEvents() {
    std::unique_lock<std::mutex> lock(pendingEventsMutex_);
    auto checkNonZeroSize = [this]() {
        return pendingEvents_.size() > 0;
    };

    if (!pendingEventsNotifier_.wait_for(lock, kPendingTimeout, checkNonZeroSize)) {
        return;
    }

    std::lock_guard<std::mutex> lockListeners(listenersMutex_);

    for (auto curr = pendingEvents_.begin(); curr != pendingEvents_.end(); curr++) {
        (*curr)();
    }

    pendingEvents_.clear();
}
}
}
}