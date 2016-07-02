#pragma once

#include "pages/MainPage.g.h"
#include "JsApp.h"

namespace voicemynews {
namespace app {
namespace win10 {
using voicemynews::app::win10::js::JsApp;

/**
 * \brief This class provides the main page of the application.
 *
 * The main page uses a layout where we display a hamburger menu on the left side and a content region on the right
 * side.
 */
public ref class MainPage sealed
{
public:
    /**
     * \brief This property determines if the menu is visibile or not.
     *
     * It can also be used for changing the menu visibility.
     */
    property bool IsMenuVisible {
        bool get();
        private: 
            void set(bool value);
    }

    /**
     * \brief This property obtains the current js backend instance which can be used by components.
     */
    property JsApp^ JsBackend {
        JsApp^ get();
        void set(JsApp^ value);
    }

public:
    MainPage();

#ifndef _DEBUG
private:
#else
public:
#endif
    /**
     * \brief This method updated the menu expansion based on the events received from MainMenu component.
     */
    void OnMenuExpanded(bool isMenuExpanded);
};
}
}
}
