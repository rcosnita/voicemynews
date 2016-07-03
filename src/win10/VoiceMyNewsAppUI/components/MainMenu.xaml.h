//
// MainMenu.xaml.h
// Declaration of the MainMenu class
//

#pragma once

#include "components\MainMenu.g.h"
#include "converters/JsonObjectConverter.h"
#include "JsApp.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace components {
using Platform::String;
using voicemynews::app::win10::js::JsApp;

using Windows::Data::Json::IJsonObject;
using Windows::Data::Json::IJsonValue;
using Windows::Data::Json::JsonArray;
using Windows::Foundation::Collections::IVector;
using Windows::UI::Xaml::DependencyObject;
using Windows::UI::Xaml::DependencyPropertyChangedEventArgs;

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
     * \brief This property allows the injection of JsBacked which can be used by the main menu.
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
    property IVector<IJsonObject^>^ MenuItems {
        IVector<IJsonObject^>^ get();
        private:
            void set(IVector<IJsonObject^>^ value);
    }

public:
    /**
     * \brief This event allows consumers of this component to determine the menu is expanded / collapsed.
     */
    event OnMenuExpanded^ MenuExpanded;

public:
    MainMenu();

#ifndef _DEBUG
private:
#else
public:
#endif
    /**
     * \brief this method expands / collapses menu. It simply negates the current expanded state.
     */
    void OpenMenu(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

    /**
     * brief this method wires the js menu model into the control.
     *
     * It does this asynchronously by waiting for menu loaded event and updating MenuItems property.
     */
    void WireJsMenuModel();
};

}
}
}
}