#ifndef VoiceMyNewsAndroid_bindings_events_EventLoopPlatform_H_
#define VoiceMyNewsAndroid_bindings_events_EventLoopPlatform_H_

#include "events/EventLoop.h"
#include "v8.h"

#include <jni.h>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <vector>

namespace voicemynews {
namespace core {
namespace events {
/**
 * \brief Provides the signature for deferred tasks queued in the event loop.
 */
typedef std::function<void()> EventLoopJsTask;

/**
 * \brief Provides a model of a task which is executed with delay.
 */
class EventLoopDelayedTask
{
public:
    EventLoopDelayedTask(EventLoopJsTask task, long cycles);

    EventLoopJsTask Task() const;

    long Cycles() const;

    /**
     * \brief Allows the decrease of the delay with a given period.
     */
    void DecreaseCycles(long cyclesCount);

private:
    EventLoopJsTask task_;
    long cycles_;
};

/**
 * \brief Provides the logic for obtaining specifc instances of event loop for the android platform.
 */
class EventLoopPlatform : public EventLoop {
public:
    using EventData = voicemynews::core::events::EventData<std::string>;

    EventLoopPlatform(bool processImmediate);

    /**
     * \brief Provides the logic for wiring the event loop platform to javascript engine.
     *
     * We need this in order to have a communication channel between java vm and app business logic.
     */
    static void WireToJs(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> obj);

    /**
     * \brief Provides the logic for obtaining an event loop instance.
     */
    static EventLoopPlatform* GetInstance();

    /**
     * \brief Provides the logic for obtaining an event instance which carries data around.
     *
     * See com/voicemynews/core/bindings/events/EventDataBindingNative class for more details.
     */
    static jobject BuildEvent(JNIEnv* env, std::string data);

public:
    /**
     * \brief Provides the algorithm for registering native handlers on a specific event.
     *
     * It returns the unique computed key for the given listener. The key can be used during deregister phase.
     */
    jstring On(JNIEnv* env,
               jstring evtName,
               jobject evtHandler);

    /**
     * \brief Provides a simple mechanism for removing a previosly registered handler.
     */
    void Off(std::string handlerKey);

    /**
     * \brief Provides a wrapper over the standard emit implementation which can be used in Java.
     */
    void Emit(JNIEnv* env,
              jstring evtName,
              jobject evtData);

    /**
     * \brief Provides an override of process events method so that we can allow deferred tasks execution.
     */
    virtual void ProcessEvents() override;

    /**
     * \brief Provides the mechanism for enqueueing tasks which are executed during next process events loop.
     */
    void EnqueueTask(EventLoopJsTask task);

    /**
     * \brief Provides the mechanism for executing the task after the given number of cycles.
     *
     * We use this mechanism especially for scheduling garbage collection of objects shared between jvm / c++ / js.
     */
    void EnqueueDelayedTask(EventLoopJsTask task, long cycles);

private:
    struct ListenerModel
    {
        std::string evtName;
        void* handlerPtr;

        ListenerModel() { }

        ListenerModel(std::string evtName_, void* handlerPtr_)
            : evtName(evtName_),
              handlerPtr(handlerPtr_)
        {
        }
    };

private:
    /**
     * \brief This method constructs a unique string identifier for the given event handler pointer.
     */
    static std::string GetStrForEventHandlerPtr(JNIEnv *env,
        std::string evtName,
        jclass handlerClass,
        jobject evtHandler);

private:
    bool processImmediate_;
    JavaVM* javaVM_ = nullptr;
    std::mutex registeredListenersMutex_;
    std::map<std::string, ListenerModel> registeredListeners_;
    std::queue<EventLoopJsTask> deferredTasks_;
    std::vector<EventLoopDelayedTask> delayedTasks_;
};
}
}
}

#endif /* VoiceMyNewsAndroid_bindings_events_EventLoopPlatform_H_ */
