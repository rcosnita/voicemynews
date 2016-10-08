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
     * \brief This event is emitted by the native part to the business logic in order to trigger categories load.
     */
    const std::string kCategoriesGet = "js:categories:get";

    /**
     * \brief This event is emitted by the business logic to the native part whenever categories load required finished.
     */
    const std::string kCategoriesGetLoaded = "js:categories:get:loaded";

    /**
     * \brief This event is emitted by the business logic so that preferences page can be opened.
     */
    const std::string kMenuItemOpenPreferences = "js:menuitems:openPreferences";

    /**
     * \brief This event is emitted by the business logic so that genius news page can be opened.
     */
    const std::string kMenuItemOpenGeniusNews = "js:menuitems:openGenius";

    /**
     * \brief This event is emitted by the business logic so that news from preferred categories can be fetched.
     */
    const std::string kNewsFetchFromPreferredCategories = "js:news:get:from_preferred_categories";

    /**
     * \brief This event is emitted by the business logic so that native part can display the news to the ui.
     */
    const std::string kNewsFetchFromPreferredCategoriesLoaded = "js:news:get:from_preferred_categories:loaded";

    /**
     * \brief This event is emitted by the native part whenever it is needed to obtain the content of a news.
     */
    const std::string kNewsFetchByUrl = "js:news:get:byurl";

    /**
     * \brief This event is emitted by the business logic whenever the content of a news has been loaded.
     */
    const std::string kNewsFetchByUrlLoaded = "js:news:get:byurl:loaded";

    /**
     * \brief This event is emitted by the native app in order to tell the business logic to start read the given news.
     */
    const std::string kNewsVoiceRead = "js:news:voice:read";

    /**
    * \brief This event is emitted by the native app in order to tell the business logic to start read the given playlist.
    */
    const std::string kNewsVoiceReadPlaylist = "js:news:voice:read:playlist";

    /**
     * \brief This event is emitted by the native app in order to tell the business logic to pause the reading.
     */
    const std::string kNewsVoiceReadPlaylistPause = "js:news:voice:read:playlist:pause";

    /**
     * \brief This event is emitted by the native app in order to tell the business logic to resume the reading.
     */
    const std::string kNewsVoiceReadPlaylistResume = "js:news:voice:read:playlist:resume";
}
}
}

#endif /* VoiceMyNewsCore_core_events_EventNames_H */
