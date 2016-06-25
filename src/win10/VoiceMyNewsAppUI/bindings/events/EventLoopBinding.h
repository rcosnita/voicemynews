#ifndef VoiceMyNewsAppUI_bindings_events_EventLoopBinding_H_
#define VoiceMyNewsAppUI_bindings_events_EventLoopBinding_H_

#include "events/EventLoop.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
using Platform::String;
using voicemynews::core::events::EventLoop;

ref class EventDataBinding;

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
#endif /* VoiceMyNewsAppUI_bindings_events_EventLoopBinding_H_ */
