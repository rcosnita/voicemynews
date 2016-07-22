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

    /**
     * \brief This event is emitted by the native part in order to ask the business logic to add the main menu.
     */
    const std::string kAppNavigationMenuLoad = "app:navigation_menu:load";

    /**
     * \brief This event is emitted by the business logic in order to tell the native part which is the main menu.
     */
    const std::string kAppNavigationMenuLoaded = "app:navigation_menu:loaded";

    /**
     * \brief This event is emitted by the business logic so that preferences page can be opened.
     */
    const std::string kMenuItemOpenPreferences = "js:menuitems:openPreferences";

    /**
     * \brief This event is emitted by the business logic so that genius news page can be opened.
     */
    const std::string kMenuItemOpenGeniusNews = "js:menuitems:openGenius";
}
}
}

#endif /* VoiceMyNewsCore_core_events_EventNames_H */
