package com.voicemynews.core.bindings.events;

/**
 * Provides an event data model which can be used by the app java code. The event can carry string
 * data between app js business logic and java code.
 */
public class EventDataBindingNative {
    private final String evtData;

    /**
     * Provides a pointer to the native event data object kept in C++.
     */
    private final long nativeEvtDataPtr;

    private EventDataBindingNative(String evtData, long nativeEvtDataPtr) {
        this.evtData = evtData;
        this.nativeEvtDataPtr = nativeEvtDataPtr;
    }

    public String getEvtData() {
        return evtData;
    }

    /**
     * Provides the logic for obtaining a new instance of event data binding object. The logic
     * it's implemented in native part because we also need to allocate some C++ objects to backup
     * the java objects.
     * @return The newly created instance.
     */
    public static native EventDataBindingNative getInstanceNative(String evtData);
}
