#ifndef VoiceMyNewsCore_core_events_EventNames_H
#define VoiceMyNewsCore_core_events_EventNames_H

#include <string>

namespace voicemynews {
namespace core {
namespace events {
    /**
     * \brief This event is emitted by js business logic after it was initialized correctly.
     */
    const std::string kAppJsStart = "app:js:start";

    /**
     * \brief This event is emitted by js business logic after it was shutdown correctly.
     */
    const std::string kAppJsShutdown = "app:js:shutdown";
}
}
}

#endif /* VoiceMyNewsCore_core_events_EventNames_H */
