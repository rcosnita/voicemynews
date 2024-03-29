﻿#ifndef VoiceMyNewsAppUI_components_CategoriesPreferences_H_
#define VoiceMyNewsAppUI_components_CategoriesPreferences_H_

#include "components/CategoriesPreferences.g.h"
#include "JsApp.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace components {
[Windows::Foundation::Metadata::WebHostHidden]
public ref class CategoriesPreferences sealed
{
using DependencyObject = Windows::UI::Xaml::DependencyObject;
using DependencyPropertyChangedEventArgs = Windows::UI::Xaml::DependencyPropertyChangedEventArgs;
using EventDataBinding = voicemynews::app::win10::bindings::events::EventDataBinding;
using IJsonObject = Windows::Data::Json::IJsonObject;
using CategoriesVector = Windows::Foundation::Collections::IVector<IJsonObject^>;
using JsApp = voicemynews::app::win10::js::JsApp;
public:
    /**
    * \brief This property allows the injection of JsBackend which can be used by the categories preferences.
    *
    * Categories preferences relies on the js backend to provide the list of available categories from which users can
    * choose.
    */
    property JsApp^ JsBackend {
        JsApp^ get();
        void set(JsApp^ value);
    }

    /**
     * \brief This property allows the injection of categories into the current component.
     */
    property CategoriesVector^ Categories {
        CategoriesVector^ get();
        void set(CategoriesVector^ value);
    }

public:
    CategoriesPreferences();

    CategoriesPreferences(JsApp^ jsBackend);

    /**
     * \brief This method is invoked automatically once js backend property has changed.
     */
    static void OnJsBackendChanged(DependencyObject^ d, DependencyPropertyChangedEventArgs^ args);

private:
    /**
     * Holds the identifier of the last registered listener which handles categories loaded events coming from js.
     * It can be used to unregister the listener once the user navigates away from the page.
     */
    Platform::String^ onCategoriesLoadedId;

private:
    /**
     * \brief Initializes the control and correctly configures the data context.
     */
    void InitControl(JsApp^ jsBackend);

    /**
     * \brief Wires the events used for displaying the data. In addition, it also emits get categories events to business logic.
     */
    void WireEvents();

    /**
     * \brief Handles the event which tells that categories have been loaded.
     */
    void OnCategoriesLoaded(EventDataBinding^ evtData);

    /**
     * \brief This method cleans up all callbacks registered to js backend.
     */
    void OnUnloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};
}
}
}
}
#endif /* VoiceMyNewsAppUI_components_CategoriesPreferences_H_ */