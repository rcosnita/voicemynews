#ifndef VoiceMyNewsAppUI_pages_GeniusNewsPage_H_
#define VoiceMyNewsAppUI_pages_GeniusNewsPage_H_

#include "bindings/events/EventDataBinding.h"
#include "pages/GeniusNewsPage.g.h"
#include "JsApp.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace pages {
/**
 * \brief This class provides the genius news page logic.
 * It fetches news from preferred categories using javascript business logic.
 */
[Windows::Foundation::Metadata::WebHostHidden]
public ref class GeniusNewsPage sealed
{
using DependencyPropertyChangedEventArgs = Windows::UI::Xaml::DependencyPropertyChangedEventArgs;
using EventDataBinding = voicemynews::app::win10::bindings::events::EventDataBinding;
using IJsonArray = Windows::Data::Json::JsonArray;
using IJsonObject = Windows::Data::Json::IJsonObject;
using JsApp = voicemynews::app::win10::js::JsApp;
using NewsVector = Windows::Foundation::Collections::IVector<IJsonObject^>;
public:
    /**
     * \brief This property allows the injection of JsBackend which can be used by the categories preferences.
     *
     * Genius news relies on the js backend to provide the list of available news currently available for the users.
     */
    property JsApp^ JsBackend {
        JsApp^ get();
        void set(JsApp^ value);
    }

    /**
     * \brief This property provides the news which must be displayed in this section.
     */
    property NewsVector^ News {
        NewsVector^ get();
        void set(NewsVector^ model);
    }

public:
    GeniusNewsPage();

    GeniusNewsPage(JsApp^ jsBackend);

    /**
     * \brief This method is invoked automatically once js backend property has changed.
     */
    static void OnJsBackendChanged(DependencyObject^ d, DependencyPropertyChangedEventArgs^ args);

protected:
    /**
     * \brief We override this method in order to do some cleanup. We remove all listenered registered to js backend.
     */
    void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

private:
    /**
     * \brief This member keeps the reference identifier of the last listener registered to handle news loaded. It is removed
     * when user navigates to a new page.
     */
    Platform::String^ onNewsLoadedId_;

    /**
     * This member keeps the json representation of the news currently listed.
     */
    Platform::String^ jsonNewsStr_;

private:
    /**
     * Initializes the page instance.
     */
    void InitPage(JsApp^ jsBackend);

    /**
     * \brief Wires js events necessary to load the preferred news from the business logic.
     */
    void WireJsModel();

    /**
     * \brief This method displays from an event to the page.
     */
    void DisplayNews(EventDataBinding^ evtData);

    /**
     * \brief This methods obtains the selected item and navigate to the page which can display the news.
     */
    void NewsLst_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);

    /**
     * \brief Provides the logic for playing the currently loaded genius news playlist.
     *
     * All the hard part is implemented in javascript. Here we just dispatch an event to the app business logic
     * requesting a new reading session.
     */
    void BtnRead_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);


    void BtnPause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};
}
}
}
}
#endif /* VoiceMyNewsAppUI_pages_GeniusNewsPage_H_ */