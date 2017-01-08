#include "pch.h"

#ifndef VOICEMYNEWSAPPUI_BINDINGS_ANALYTICS_WEBBROWSERBINDING_
#define VOICEMYNEWSAPPUI_BINDINGS_ANALYTICS_WEBBROWSERBINDING_

#include "analytics/Analytics.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace analytics {
/**
 * \brief Provides a simple wrapper over a web browser.
 *
 * This is the platform specific implementation which can be used for analytics.
 */
public ref class WebBrowserBinding sealed
{
delegate void DelayedEventExecution();
using WebView = Windows::UI::Xaml::Controls::WebView;
using String = Platform::String;
using DelayedEventExecutionList = Platform::Collections::Vector<DelayedEventExecution^>;

public:
    WebBrowserBinding();

    /**
     * \brief Provides the logic for loading the given web page content into the underlining browser.
     */
    void LoadContent(String^ webPageContent);

    /**
     * \brief Provides the logic for posting the event data into the underlining browser as a post message.
     */
    void SendEvent(String^ evtName, String^ evtData);

private:
    /**
     * \brief This callback is wired to the web browser navigation in order to correctly set the internal state of the browser wrapper.
     */
    void OnNavigationCompleted(WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs^ args);

    /**
     * \brief Provides a simple implementation for running actions on ui thread.
     *
     * This is mandatory, because analytics events can be generated from various threads and we must dispatch them
     * to the ui thread.
     */
    void RunOnUiThread(DelayedEventExecution^ action);

private:
    WebView^ webBrowser_;
    bool ready_;
    DelayedEventExecutionList^ delayedEvents_;
};

/**
 * \brief Provides a native web browser implementation which can be used in core when necessary.
 */
class WebBrowserBindingNative : public voicemynews::core::analytics::WebBrowser
{
public:
    void LoadContent(std::string webPageContent) override;

    void SendEvent(std::string evtName, std::string evtData) override;

private:
    WebBrowserBindingNative(WebBrowserBinding^ browser);
    friend class voicemynews::core::analytics::WebBrowser;

private:
    WebBrowserBinding^ browser_;
};
}
}
}
}
}

#endif /* VOICEMYNEWSAPPUI_BINDINGS_ANALYTICS_WEBBROWSERBINDING_ */