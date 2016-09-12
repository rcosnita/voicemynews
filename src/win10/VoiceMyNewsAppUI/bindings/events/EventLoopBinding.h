#ifndef VoiceMyNewsAppUI_bindings_events_EventLoopBinding_H_
#define VoiceMyNewsAppUI_bindings_events_EventLoopBinding_H_

#include "EventDataBinding.h"
#include "events/EventData.h"
#include "events/EventLoop.h"

#include <mutex>
#include <queue>

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
/**
 * \brief This delegate defines a winrt method which can be binded from javascript.
 */
public delegate void EventHandler(EventDataBinding^);

/**
 * \brief This delegate defines the api for tasks which can be enqueued for deferred execution.
 */
public delegate void JsLoopEnqueuedTask();

/**
 * \class EventLoopBinding
 * \brief Runtime component used to grant js access to the app event loop.
 * The application relies on the event loop in order to have a bridge between native XAML world and js business logic.
 */
public ref class EventLoopBinding sealed {
using String = Platform::String;
using EventLoop = voicemynews::core::events::EventLoop;
using EventData = voicemynews::core::events::EventData<std::string>;

public:
    EventLoopBinding();

    /**
     * \brief This method enforces each new emitted event to trigger a process event invocation. Extremely useful in tests.
     */
    EventLoopBinding(bool processImmediate);

    /**
     * \brief This method queues a new event under evtName space.
     *
     * \param evtName The event name emitted.
     * \param evtData The event data associated with this event.
     */
    void Emit(String^ evtName, EventDataBinding^ evtData);

    /**
     * \brief This method allows developers to register for the specified event.
     *
     * If the developer intends to remove the newly added handler, he must save the handler id and
     * pass it to off method when necessary.
     */
    String^ On(String^ evtName, EventHandler^ handler);

    /**
     * \brief This method allows developers to unregister a handler from the specified event.
     */
    void Off(String^ handlerId);

    /**
     * \brief This method allows the app to process the latest queued events.
     */
    void ProcessEvents();

    /**
     * \brief This method allows the app to enqueue deferred tasks which are going to be processed at the next process events loop.
     */
    void EnqueueTask(JsLoopEnqueuedTask^ task);

public:
    /**
     * \brief This method obtains an event loop instance. It is a singleton event loop we can use in the application.
     */
    static EventLoopBinding^ GetInstance();

private:
    struct ListenerModel
    {
        String^ evtName;
        void* handlerPtr;

        ListenerModel() { }

        ListenerModel(String^ evtName_, void* handlerPtr_)
            : evtName(evtName_),
            handlerPtr(handlerPtr_)
        {
        }
    };

private:
    EventLoop eventLoop_;
    bool processImmediate_ = false;
    std::mutex registeredListenersMutex_;
    std::map<std::string, ListenerModel> registeredListeners_;
    std::queue<JsLoopEnqueuedTask^> deferredTasks_;
};
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace events {
using Platform::String;
using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::bindings::events::EventLoopBinding;

/**
 * \brief This class provides a factory for obtaining EventLoop instances which are platform specific.
 *
 * JS code can safely rely on this in order to implement application main event loop.
 */
public ref class EventLoopPlatform sealed {
public:
    /**
     * \brief This method obtains an event loop instance.
     */
    static EventLoopBinding^ GetInstance();

    /**
     * \brief This method builds an event instance which is compatible with application event loop.
     */
    static EventDataBinding^ BuildEvent(String^ data);
};
}
}
}
#endif /* VoiceMyNewsAppUI_bindings_events_EventLoopBinding_H_ */
