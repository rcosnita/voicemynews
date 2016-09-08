#ifndef VoiceMyNewsAppUI_components_MainMenu_H_
#define VoiceMyNewsAppUI_components_MainMenu_H_

#include "components/MainMenu.g.h"
#include "JsApp.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace components {
/**
 * This delegate method can be used in order to handle menu expanded events.
 */
public delegate void OnMenuExpanded(bool isMenuExpanded);

/**
 * \brief This class provides the main navigation menu for voicemynews.
 */
[Windows::Foundation::Metadata::WebHostHidden]
public ref class MainMenu sealed
{
using String = Platform::String;
using JsApp = voicemynews::app::win10::js::JsApp;
using EventDataBinding = voicemynews::core::events::EventDataBinding;

using IJsonObject = Windows::Data::Json::IJsonObject;
using IJsonValue = Windows::Data::Json::IJsonValue;
using JsonArray = Windows::Data::Json::JsonArray;
using DependencyObject = Windows::UI::Xaml::DependencyObject;
using DependencyPropertyChangedEventArgs = Windows::UI::Xaml::DependencyPropertyChangedEventArgs;

public:
    /**
     * \brief This property determines if the menu is expanded or not.
     *
     * It can also be used for changing the menu visibility.
     */
    property bool IsMenuExpanded {
        bool get();
        void set(bool value);
    }

    /**
     * \brief This property allows the injection of JsBackend which can be used by the main menu.
     *
     * Main menu relies on the js backend to provide the menu items which must be displayed to the users.
     */
    property JsApp^ JsBackend {
        JsApp^ get();
        void set(JsApp^ value);
    }

    /**
     * \brief This property holds all the menu items which must be displayed in the hamburger menu.
     */
    property Windows::Foundation::Collections::IVector<IJsonObject^>^ MenuItems {
        Windows::Foundation::Collections::IVector<IJsonObject^>^ get();
        private:
            void set(Windows::Foundation::Collections::IVector<IJsonObject^>^ value);
    }

    /**
     * \brief This method is invoked automatically once js backend property has changed.
     */
    static void OnJsBackendChanged(DependencyObject^ d, DependencyPropertyChangedEventArgs^ args);

public:
    /**
     * \brief This event allows consumers of this component to determine the menu is expanded / collapsed.
     */
    event OnMenuExpanded^ MenuExpanded;

public:
    MainMenu();

    MainMenu(JsApp^ jsApp);

    /**
     * \brief Programmatically click on the expand / collapse button from the main menu. It comes handy in tests.
     */
    void DoClickExpandButton();

private:
    /**
     * \brief This method expands / collapses menu. It simply negates the current expanded state.
     */
    void OpenMenu(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

    /**
     * \brief This method is invoked when the navigation menu is loaded by the business logic.
     */
    void OnMenuLoaded(EventDataBinding^ evtData);

    /**
     * \brief This method wires the js menu model into the control.
     *
     * It does this asynchronously by waiting for menu loaded event and updating MenuItems property.
     */
    void WireJsMenuModel();
};

}
}
}
}
#endif /* VoiceMyNewsAppUI_components_MainMenu_H_ */