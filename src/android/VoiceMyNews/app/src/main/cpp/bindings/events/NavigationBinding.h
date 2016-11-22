#ifndef VoiceMyNewsAndroid_bindings_events_NavigationBinding_H_
#define VoiceMyNewsAndroid_bindings_events_NavigationBinding_H

#include "v8.h"

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace events {
/**
 * \brief Provides the android navigation manager.
 *
 * This layer provides all code required to navigate between various sections of the application.
 */
class NavigationBinding
{
public:
    NavigationBinding() = default;
    virtual ~NavigationBinding() = default;

public:
    /**
     * \brief Provides the code which wires the current navigation manager implementation to v8 engine.
     */
    static void WireToJs(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> obj);
};
}
}
}
}
}

#endif /* VoiceMyNewsAndroid_bindings_events_NavigationBinding_H */