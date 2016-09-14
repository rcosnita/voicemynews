#ifndef VoiceMyNewsAppUI_components_MainMenuItem_H_
#define VoiceMyNewsAppUI_components_MainMenuItem_H_

#include "JsApp.h"
#include "converters/JsonObjectConverter.h"
#include "components\MainMenuItem.g.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace components {
/**
    * \brief This class provides a control for rendering menu items which are described by a JSON object.
    */
[Windows::Foundation::Metadata::WebHostHidden]
public ref class MainMenuItem sealed
{
using IJsonObject = Windows::Data::Json::IJsonObject;
using JsApp = voicemynews::app::win10::js::JsApp;

public:
    /**
    * \brief This property allows the injection of JsBacked which can be used by the main menu item.
    */
    property JsApp^ JsBackend {
        JsApp^ get();
        void set(JsApp^ value);
    }

public:
    MainMenuItem();

    /**
     * \brief Programmatically click on the current menu item. It is extremely useful in tests but it might come handy in other scenarios as well.
     */
    void DoClick();

private:
    /**
     * \brief This method is invoked when user clicks on a menu item. Internally, it delegates an event to the js backend.
     */
    void OnMenuItemClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};
}
}
}
}
#endif /* #ifndef VoiceMyNewsAppUI_components_MainMenuItem_H_ */