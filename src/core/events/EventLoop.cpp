#include "EventLoop.h"

namespace voicemynews {
namespace core {
namespace events {
const std::chrono::duration<float> EventLoop::kPendingTimeout = std::chrono::milliseconds(1);

void EventLoop::Off(std::string evtName, void* fnPointer) {
    std::lock_guard<std::mutex> lock(listenersMutex_);

    auto listenersPos = listeners_.find(evtName);
    if (listenersPos == listeners_.end()) {
        return;
    }

    auto fnPointerPos = listenersRegisteredAssoc_.find(evtName)->second->find(fnPointer);
    if (fnPointerPos == listenersRegisteredAssoc_.find(evtName)->second->end()) {
        return;
    }

    auto listenerPos = std::find(listeners_.find(evtName)->second->begin(),
                            listeners_.find(evtName)->second->end(),
                            fnPointerPos->second);

    delete *listenerPos;
    listeners_.find(evtName)->second->erase(listenerPos);
    listenersRegisteredAssoc_.at(evtName)->erase(fnPointerPos);
}

int EventLoop::GetListenersCount(std::string evtName) const {
    auto listenersPos = listeners_.find(evtName);

    return listenersPos == listeners_.end() ? 0 : static_cast<int>(listenersPos->second->size());
}

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
    
    std::lock_guard<std::mutex> lockEmmiter(emitterMutex_);
    for (auto curr = pendingEventsTemp_.begin(); curr != pendingEventsTemp_.end(); curr++) {
        pendingEvents_.push_back(*curr);
    }

    pendingEventsTemp_.clear();
    pendingEventsNotifier_.notify_all();
}

bool EventLoop::InternalRegisteredMethod::operator==(const InternalRegisteredMethod& obj) {
    return id_ == obj.id_;
}

void EventLoop::InternalRegisteredMethod::operator()(void* dataPtr) const {
    fn_(dataPtr);
}
}
}
}