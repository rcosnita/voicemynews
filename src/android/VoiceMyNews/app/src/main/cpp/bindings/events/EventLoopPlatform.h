#ifndef VoiceMyNewsAndroid_events_EventLoopPlatform_H_
#define VoiceMyNewsAndroid_events_EventLoopPlatform_H_

#include <map>
#include <mutex>
#include <jni.h>

#include "events/EventLoop.h"

namespace voicemynews {
namespace core {
namespace events {
/**
 * \brief Provides the logic for obtaining specifc instances of event loop for the android platform.
 */
class EventLoopPlatform : EventLoop {
public:
    using EventData = voicemynews::core::events::EventData<std::string>;

    EventLoopPlatform(bool processImmediate);

    /**
     * \brief Provides the logic for obtaining an event loop instance.
     */
    static std::shared_ptr<EventLoopPlatform> GetInstance();

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
};
}
}
}

#endif /* VoiceMyNewsAndroid_events_EventLoopPlatform_H_ */
