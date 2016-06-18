#ifndef VoiceMyNewsCore_events_EventLoop_H_
#define VoiceMyNewsCore_events_EventLoop_H_

#include "EventData.h"

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace voicemynews {
namespace core {
namespace events {
/**
 * \brief This class provides an event loop mechanism which queue events and process them on demand.
 */
class EventLoop {
public:
    EventLoop() = default;

    virtual ~EventLoop() = default;

    /**
     * \brief This method provides a way to register handlers for specific events.
     *
     * \param evtHandler the event handler function we want to register.
     */
    template<typename T>
    void On(std::string evtName, std::function<void(std::shared_ptr<EventData<T>>)> evtHandler) {
        std::unique_lock<std::mutex> lock(listenersMutex_);
        if (listeners_.find(evtName) == listeners_.end()) {
            listeners_[evtName] = std::make_unique<std::vector<std::function<void(void*)>>>();
        }

        listeners_[evtName]->push_back([evtHandler](void* evtData) {
            auto evtDataCasted = reinterpret_cast<std::shared_ptr<EventData<T>>*>(evtData);
            evtHandler(*evtDataCasted);
        });
    }

    /**
     * \brief This method provides a way to emit events on the current queue.
     */
    template<typename T>
    void Emit(std::string evtName, std::shared_ptr<EventData<T>> evtData) {
        if (listeners_.find(evtName) == listeners_.end()) {
            return;
        }

        std::lock_guard<std::mutex> lock(pendingEventsMutex_);

        pendingEvents_.push_back([evtName, evtData, this]() {
            auto localDataCopy = evtData;
            auto listenerPos = listeners_.find(evtName);
            void* dataPtr = &localDataCopy;

            for (auto curr = listenerPos->second->begin(); curr != listenerPos->second->end(); curr++) {
                (*curr)(dataPtr);
            }
        });

        pendingEventsNotifier_.notify_all();
    }

    /**
     * \brief This method provides a mechanism for processing pending events.
     *
     * Internally it uses a notification based mechanism and becomes blocking till events are available. Like this,
     * we can avoid polling intervals and other mechanism which are killers for performance.
     */
    void ProcessEvents();

private:
    static const std::chrono::duration<float> kPendingTimeout;

    std::mutex listenersMutex_;
    std::mutex pendingEventsMutex_;
    std::condition_variable pendingEventsNotifier_;

    std::map<std::string, std::unique_ptr<std::vector<std::function<void(void*)>>>> listeners_;
    std::vector<std::function<void()>> pendingEvents_;
};
}
}
}

#endif // VoiceMyNewsCore_events_EventLoop_H_
