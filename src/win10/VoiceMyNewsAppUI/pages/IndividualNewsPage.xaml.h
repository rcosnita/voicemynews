//
// IndividualNewsPage.xaml.h
// Declaration of the IndividualNewsPage class
//

#pragma once

#include "JsApp.h"
#include "converters/ParagraphSubHeadingConverter.h"
#include "pages/IndividualNewsPage.g.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace pages {
/**
 * \brief Provides the individual news reading feature. When navigated to this page, the news model is passed as argument.
 */
[Windows::Foundation::Metadata::WebHostHidden]
public ref class IndividualNewsPage sealed
{
using EventDataBinding = voicemynews::app::win10::bindings::events::EventDataBinding;
using JsApp = voicemynews::app::win10::js::JsApp;
using JsEventLoop = voicemynews::app::win10::bindings::events::EventLoopBinding;
using NewsModel = Windows::Data::Json::IJsonObject;
using ParagraphModel = Windows::Foundation::Collections::IVector<NewsModel^>;
using ContributedByModel = Windows::Foundation::Collections::IVector<Platform::String^>;
public:
    /**
     * \brief Stores the json representation of the news we want to display.
     */
    property NewsModel^ CurrNews {
        NewsModel^ get();
        private:
            void set(NewsModel^ model);
    }

    /**
     * \brief Stores an array of paragraphs which belong to the current news.
     */
    property ParagraphModel^ Paragraphs {
        ParagraphModel^ get();
        private:
            void set(ParagraphModel^ paragraphModel);
    }

    /**
     * \brief Stores an array of contributions which must be displayed.
     */
    property ContributedByModel^ ContributedBy {
        ContributedByModel^ get();
        private:
            void set(ContributedByModel^ model);
    }

public:
    IndividualNewsPage();

    IndividualNewsPage(JsApp^ jsBackend);

    /**
     * \brief provides a programmatic way to start reading the displayed news.
     */
    void ReadNews();

protected:
    /**
     * \brief This method is invoked automatically whenever the page is opened through navigation.
     */
    void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs ^e) override;

    /**
     * \brief When user navigates away from this page we clean all listeners.
     */
    void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs ^e) override;

private:
    NewsModel^ newsRssDesc_;
    JsEventLoop^ jsEventLoop_;
    Platform::String^ onNewsLoadedId_;

private:
    /**
     * \brief Once the js business logic correctly loads the news this method is solely responsible for pushing it to screen.
     */
    void DisplayNews(EventDataBinding^ evtData);

    /**
     * \brief This method initializes the page and correctly configures the data context.
     */
    void InitPage(JsApp^ jsBackend);

    /**
     * \brief We use this method to wire the callbacks required for handling individual news related events.
     */
    void WireJsEvents();

    /**
     * \brief This method triggers the read news action for the current displayed news.
     */
    void btnReadNews_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
};
}
}
}
}
