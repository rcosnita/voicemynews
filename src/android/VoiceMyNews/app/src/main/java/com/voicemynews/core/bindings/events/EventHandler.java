package com.voicemynews.core.bindings.events;

/**
 * Provides the contract which allows java code to register listeners in the native event loop.
 */
public interface EventHandler {
    /**
     * Provides the algorithm for handling an emitted event. The event data stores all revelant
     * information received from the js business logic.
     *
     * @param evtData The event data received from event.
     */
    void handleEvent(EventDataBinding evtData);
}
