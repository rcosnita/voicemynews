package com.voicemynews.core.bindings.events;

/**
 * Provides the event loop contract which java code from the app is relying on. We mainly use the
 * interface, because during tests we want to be able to mock the event loop rather than using the
 * native one.
 */
public interface EventLoopBinding {
    /**
     * Enqueue an action which will notify all event listeners about the newly available data.
     * The event loop we are currently using is mainly controlled from app business logic (js).
     *
     * @param evtName The event name we want to emit.
     * @param evtData The event data we want to send to all listeners.
     */
    void emit(String evtName, EventDataBinding evtData);

    /**
     * Provides the logic for adding a new listener to the specified event name.
     *
     * @param evtName The event name we want to listen to.
     * @param evtHandler The action we want to execute every time the event is raised.
     * @return A unique listener identifier which can later be used during off / unsubscribe phase.
     */
    String on(String evtName, EventHandler evtHandler);

    /**
     * Provides the logic for unsubscribing a listener from the event where it was registered.
     * This method will usually be invoked during cleanup or transitions.
     *
     * @param listenerId The unique listener identifier obtained when subscribing to an event.
     */
    void off(String listenerId);
}
