#ifndef VoiceMyNewsAppUI_bindings_events_EventLoopBinding_H_
#define VoiceMyNewsAppUI_bindings_events_EventLoopBinding_H_

#include "EventDataBinding.h"
#include "events/EventLoop.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
using Platform::String;
using voicemynews::core::events::EventLoop;

/**
 * \brief This delegate defines a winrt method which can be binded from javascript.
 */
public delegate void EventHandler(EventDataBinding^);

/**
 * \class EventLoopBinding
 * \brief Runtime component used to grant js access to the app event loop.
 * The application relies on the event loop in order to have a bridge between native XAML world and js business logic.
 */
public ref class EventLoopBinding sealed {
public:
    /**
     * \brief This method queues a new event under evtName space.
     *
     * \param evtName The event name emitted.
     * \param evtData The event data associated with this event.
     */
    void Emit(String^ evtName, EventDataBinding^ evtData);

    /**
     * \brief This method allows developers to register for the specified event.
     */
    void On(String^ evtName, EventHandler^ handler);

    /**
     * \brief This method allows the app to process the latest queued events.
     */
    void ProcessEvents();

public:
    /**
     * \brief This method obtains an event loop instance. It is a singleton event loop we can use in the application.
     */
    static EventLoopBinding^ GetInstance();

private:
    EventLoop eventLoop_;
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
