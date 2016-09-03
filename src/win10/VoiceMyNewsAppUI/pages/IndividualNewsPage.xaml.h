//
// IndividualNewsPage.xaml.h
// Declaration of the IndividualNewsPage class
//

#pragma once

#include "JsApp.h"
#include "pages\IndividualNewsPage.g.h"

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
    Platform::String^ onNewsLoadedId;
};
}
}
}
}
