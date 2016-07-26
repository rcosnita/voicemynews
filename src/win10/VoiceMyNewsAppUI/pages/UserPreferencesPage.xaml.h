#ifndef VoiceMyNewsAppUI_pages_UserPreferencesPage_H_
#define VoiceMyNewsAppUI_pages_UserPreferencesPage_H_
#include "pages\UserPreferencesPage.g.h"
#include "JsApp.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace pages {
/**
 * \brief This class provides the landing page for configuring user preferences.
 *
 * In this pages, user will be able to configure news desired categories and also set fetching preferences.
 */
[Windows::Foundation::Metadata::WebHostHidden]
public ref class UserPreferencesPage sealed
{
using JsApp = voicemynews::app::win10::js::JsApp;
public:
    /**
    * \brief This property obtains the current js backend instance which can be used by components.
    */
    property JsApp^ JsBackend {
        JsApp^ get();
        void set(JsApp^ value);
    }

public:
    UserPreferencesPage();
};
}
}
}
}
#endif /* VoiceMyNewsAppUI_pages_UserPreferencesPage_H_ */