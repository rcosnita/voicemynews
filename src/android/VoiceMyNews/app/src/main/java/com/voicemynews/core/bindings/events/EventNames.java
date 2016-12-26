package com.voicemynews.core.bindings.events;

/**
 * Provides all event names used by the application. This is the Java equivalent of C++ EventNames.h.
 * It is strongly recommended to keep these files in sync.
 */
public interface EventNames {
    String kAppJsStart = "app:js:start";

    String kAppJsShutdown = "app:js:shutdown";

    String kAppNavigationMenuLoad = "app:navigation_menu:load";

    String kAppNavigationMenuLoaded = "app:navigation_menu:loaded";

    String kCategoriesGet = "js:categories:get";

    String kCategoriesGetLoaded = "js:categories:get:loaded";

    String kMenuItemOpenPreferences = "js:menuitems:openPreferences";

    String kMenuItemOpenGeniusNews = "js:menuitems:openGenius";

    String kNewsFetchFromPreferredCategories = "js:news:get:from_preferred_categories";

    String kNewsFetchFromPreferredCategoriesLoaded = "js:news:get:from_preferred_categories:loaded";

    String kNewsFetchByUrl = "js:news:get:byurl";

    String kNewsFetchByUrlLoaded = "js:news:get:byurl:loaded";

    String kNewsVoiceRead = "js:news:voice:read";

    String kNewsVoiceReadPlaylist = "js:news:voice:read:playlist";

    String kNewsVoiceReadPlaylistPause = "js:news:voice:read:playlist:pause";

    String kNewsVoiceReadPlaylistResume = "js:news:voice:read:playlist:resume";

    String kNewsVoiceReadPlaylistSkip = "js:news:voice:read:playlist:skip";
}
