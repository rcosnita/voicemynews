#include "pch.h"
#include "WebBrowserBinding.h"
#include "utils/Conversions.h"

#include <functional>
#include <memory>

using voicemynews::app::win10::bindings::analytics::WebBrowserBindingNative;
using voicemynews::app::win10::bindings::analytics::WebBrowserBinding;

using voicemynews::app::win10::utils::ConvertStdStrToPlatform;

using Windows::UI::Xaml::Controls::WebViewExecutionMode;

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace analytics {
WebBrowserBinding::WebBrowserBinding()
{
    delayedEvents_ = ref new WebBrowserBinding::DelayedEventExecutionList();
    webBrowser_ = ref new WebBrowserBinding::WebView(WebViewExecutionMode::SeparateThread);
    ready_ = false;
}

void WebBrowserBinding::LoadContent(WebBrowserBinding::String^ webPageContent)
{
    webBrowser_->NavigationCompleted += ref new Windows::Foundation::TypedEventHandler<WebBrowserBinding::WebView^, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs ^>(this, &WebBrowserBinding::OnNavigationCompleted);
    webBrowser_->NavigateToString(webPageContent);
}

void WebBrowserBinding::SendEvent(WebBrowserBinding::String^ evtName, WebBrowserBinding::String^ evtData)
{
    auto delayedEvt = ref new DelayedEventExecution([this, evtName, evtData]() {
        WebBrowserBinding::String^ targetOrigin = "*";
        auto messageArgs = ref new Platform::Collections::Vector<WebBrowserBinding::String^>();
        messageArgs->Append(evtData);
        messageArgs->Append(targetOrigin);
        webBrowser_->InvokeScriptAsync("postMessage", messageArgs);
    });

    if (!ready_) {
        delayedEvents_->Append(delayedEvt);
        return;
    }

    RunOnUiThread(delayedEvt);
}

void WebBrowserBinding::OnNavigationCompleted(WebView^ sender, Windows::UI::Xaml::Controls::WebViewNavigationCompletedEventArgs^ args)
{
    ready_ = true;

    for (DelayedEventExecution^ delayedEvent : delayedEvents_) {
        RunOnUiThread(delayedEvent);
    }

    delayedEvents_->Clear();
}

void WebBrowserBinding::RunOnUiThread(DelayedEventExecution^ action)
{
    webBrowser_->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Low,
        ref new Windows::UI::Core::DispatchedHandler([action]() {
        action();
    }));
}

WebBrowserBindingNative::WebBrowserBindingNative(WebBrowserBinding^ browser)
    : browser_(browser)
{
}

void WebBrowserBindingNative::LoadContent(std::string webPageContent)
{
    browser_->LoadContent(ConvertStdStrToPlatform(webPageContent));
}

void WebBrowserBindingNative::SendEvent(std::string evtName, std::string evtData)
{
    browser_->SendEvent(ConvertStdStrToPlatform(evtName), ConvertStdStrToPlatform(evtData));
}
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace analytics {
    std::shared_ptr<WebBrowser> WebBrowser::GetInstance()
    {
        return std::shared_ptr<WebBrowser>(new WebBrowserBindingNative(ref new WebBrowserBinding()));
    }
}
}
}