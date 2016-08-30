#pragma once

#include "App.g.h"
#include <ppltasks.h>

namespace VoiceMyNewsApp {
using voicemynews::app::win10::js::JsApp;

/**
 * \brief This is the entry point in the voicemynews application.
 *
 * This class takes care of the app bootstrap as well as to various events received from the operating system.
 */
ref class App sealed
{
public:
    /**
     * \brief This property provides access to js backend which can be used by consumers.
     */
    property JsApp^ JsBackend {
        JsApp^ get();
    }

protected:
    virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

internal:
    App();

private:
    JsApp^ jsApp_;
    bool jsAppStarted_;
    concurrency::task<void> jsAppRunner_;

private:
    void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
    void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);
};
}