#ifndef VoiceMyNewsAppUI_pages_GeniusNewsPage_H_
#define VoiceMyNewsAppUI_pages_GeniusNewsPage_H_

#include "pages\GeniusNewsPage.g.h"
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

private:
    /**
     * \brief Wires js events necessary to load the preferred news from the business logic.
     */
    void WireJsModel();

    /**
     * \brief This method displays the news vector into the page.
     */
    void DisplayNews(IJsonArray^ newsVector);
};
}
}
}
}
#endif /* VoiceMyNewsAppUI_pages_GeniusNewsPage_H_ */