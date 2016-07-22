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
    * \brief This method provides a way to emit events on the current queue.
    *
    * \param evtName the event we want to emit.
    * \param evtData the event data we want to emit.
    */
    template<typename T>
    void Emit(std::string evtName, std::shared_ptr<EventData<T>> evtData) {
        if (listeners_.find(evtName) == listeners_.end()) {
            return;
        }

        std::unique_lock<std::mutex> lock(emitterMutex_);
        auto pendingDest = pendingEvents_.size() == 0 ? &pendingEvents_ : &pendingEventsTemp_;
        pendingDest->push_back([evtName, evtData, this]() {
            auto localDataCopy = evtData;
            auto listenerPos = listeners_.find(evtName);
            void* dataPtr = &localDataCopy;

            for (auto curr = listenerPos->second->begin(); curr != listenerPos->second->end(); curr++) {
                (*(*curr))(dataPtr);
            }
        });
    }

    /**
    * \brief This method returns the approximate number of active listeners for the specified event.
    *      The method is not thread safe.
    *
    * \param evtName the name of the event for which we count the listeners.
    */
    int GetListenersCount(std::string evtName) const;

    /**
    * \brief This method removes the given function pointer from the list of active listeners for the specified event.
    *
    * \param fnPointer a pointer to the listener we want to unregister.
    */
    void Off(std::string evtName, void* fnPointer);

    /**
     * \brief This method provides a way to register handlers for specific events.
     *
     * \param evtName the name of the event where we want to register the given handler.
     * \param evtHandler the event handler function we want to register.
     */
    template<typename T>
    void On(std::string evtName, std::function<void(std::shared_ptr<EventData<T>>)>& evtHandler) {
        std::unique_lock<std::mutex> lock(listenersMutex_);
        if (listeners_.find(evtName) == listeners_.end()) {
            listeners_[evtName] = std::make_unique<std::vector<InternalRegisteredMethod*>>();
        }

        auto registeredMethod = new InternalRegisteredMethod([evtHandler](void* evtData) {
            auto evtDataCasted = reinterpret_cast<std::shared_ptr<EventData<T>>*>(evtData);
            evtHandler(*evtDataCasted);
        });

        listeners_[evtName]->push_back(registeredMethod);

        void* evtHandlerLocation = &evtHandler;

        if (listenersRegisteredAssoc_.find(evtName) == listenersRegisteredAssoc_.end()) {
            listenersRegisteredAssoc_.emplace(evtName, std::make_unique<std::map<void*, InternalRegisteredMethod*>>());
        }

        if (listenersRegisteredAssoc_.at(evtName)->find(evtHandlerLocation) != listenersRegisteredAssoc_.at(evtName)->end()) {
            return;
        }

        listenersRegisteredAssoc_.at(evtName)->emplace(evtHandlerLocation, registeredMethod);
    }

    /**
     * \brief This method provides a mechanism for processing pending events.
     *
     * Internally it uses a notification based mechanism and becomes blocking till events are available. Like this,
     * we can avoid polling intervals and other mechanism which are killers for performance.
     */
    void ProcessEvents();

private:
    /**
     * \brief Internal class used to store information about registered listeners.
     *
     * It can be used as a functor class so that it makes things convenient when processing events.
     */
    class InternalRegisteredMethod {
    public:
        template<typename T>
        InternalRegisteredMethod(T&& fn) {
            fn_ = fn;
        }

        bool operator==(const InternalRegisteredMethod& obj);
        void operator()(void* dataPtr) const;

    private:
        uintptr_t id_;
        std::function<void(void*)> fn_;
    };

private:
    static const std::chrono::duration<float> kPendingTimeout;

    std::mutex listenersMutex_;
    std::mutex pendingEventsMutex_;
    std::mutex emitterMutex_;
    std::condition_variable pendingEventsNotifier_;

    /**
     * \brief This member holds the association between the give handler and the registered handler for each event name.
     *
     * We need this because there are some memory moves during listeners registration.
     */
    std::map<std::string, std::unique_ptr<std::map<void*, InternalRegisteredMethod*>>> listenersRegisteredAssoc_;
    std::map<std::string, std::unique_ptr<std::vector<InternalRegisteredMethod*>>> listeners_;
    std::vector<std::function<void()>> pendingEvents_;
    std::vector<std::function<void()>> pendingEventsTemp_;
};
}
}
}

#endif // VoiceMyNewsCore_events_EventLoop_H_
