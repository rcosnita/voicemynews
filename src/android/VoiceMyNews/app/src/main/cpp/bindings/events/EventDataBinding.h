#ifndef VoiceMyNewsAndroid_events_EventDataBinding_H_
#define VoiceMyNewsAndroid_events_EventDataBinding_H_

#include <jni.h>
#include <string>
#include "events/EventData.h"

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace events {
/**
 * \brief Provides the event data model used for communicating between business logic and ui logic.
 */
class EventDataBinding : public voicemynews::core::events::EventData<std::string> {
public:
    EventDataBinding(std::string data);
};
}
}
}
}
}

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Provides the implementation for getInstanceNative method from EventDataBindingNative java class.
 *
 * It correlates a native data event with a java data event.
 */
JNIEXPORT jobject JNICALL Java_com_voicemynews_core_bindings_events_EventDataBindingNative_getInstanceNative(
    JNIEnv *env,
    jclass objCls,
    jstring evtData);

#ifdef __cplusplus
};
#endif

#endif /* VoiceMyNewsAndroid_events_EventDataBinding_H_ */
