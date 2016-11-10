package com.voicemynews.core.bindings.events;

/**
 * Provides a native event loop which can be used by app java code to listen and emit events for
 * the native app business logic. This layer provides interoperability with JS business logic. In
 * order to achieve this most of the methods are natively implemented.
 */
public class EventLoopBindingNative implements EventLoopBinding {
    private static Object currInstanceLock = new Object();
    private static EventLoopBinding currInstance;
    private long nativeEmitterPtr;

    private EventLoopBindingNative(long nativeEmitterPtr) {
        this.nativeEmitterPtr = nativeEmitterPtr;
    }

    /**
     * Enqueue an action which will notify all event listeners about the newly available data.
     * The event loop we are currently using is mainly controlled from app business logic (js).
     *
     * @param evtName The event name we want to emit.
     * @param evtData The event data we want to send to all listeners.
     */
    public native void emit(String evtName, EventDataBindingNative evtData);

    /**
     * Provides the logic for adding a new listener to the specified event name.
     *
     * @param evtName The event name we want to listen to.
     * @param evtHandler The action we want to execute every time the event is raised.
     * @return A unique listener identifier which can later be used during off / unsubscribe phase.
     */
    public native String on(String evtName, EventHandler evtHandler);

    /**
     * Provides the logic for unsubscribing a listener from the event where it was registered.
     * This method will usually be invoked during cleanup or transitions.
     *
     * @param listenerId The unique listener identifier obtained when subscribing to an event.
     */
    public native void off(String listenerId);

    /**
     * Provides a singleton factory method for obtaining an event loop. The whole app is allowed to
     * use one EventLoop.
     * @return The event loop which can be used by the application.
     */
    public static EventLoopBinding getInstance() {
        if (currInstance == null) {
            synchronized (currInstanceLock) {
                currInstance = getInstanceNative();
            }
        }
        return currInstance;
    }

    /**
     * Provides a factory method for obtaining new instances of event loop. It is provided by a native
     * implementation.
     * @return
     */
    private native static EventLoopBinding getInstanceNative();

    /**
     * Provides a helper method destroying the native event emitter.
     * @param nativeEmitterPtr the memory address of the native event emitter.
     */
    private native static void destroyNative(long nativeEmitterPtr);

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        this.destroyNative(nativeEmitterPtr);
    };
}
