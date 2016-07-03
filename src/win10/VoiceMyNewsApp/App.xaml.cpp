#include "pch.h"

#include <ppltasks.h>

#include "events/EventNames.h"
#include "utils/Conversions.h"

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using voicemynews::app::win10::MainPage;
using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;

namespace VoiceMyNewsApp {
JsApp^ App::JsBackend::get() {
    return jsApp_;
}

App::App()
{
    InitializeComponent();
    Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);

    jsApp_ = ref new JsApp();
    jsAppStarted_ = false;

    auto appStartEvt = ConvertStdStrToPlatform(voicemynews::core::events::kAppJsStart);
    jsApp_->GetEventLoop()->On(appStartEvt,
        ref new voicemynews::app::win10::bindings::events::EventHandler([this](EventDataBinding^ evtData) {
        jsAppStarted_ = true;
    }));

    jsAppRunner_ = std::thread([this]() {
        jsApp_->Start();
    });
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
#if _DEBUG
    if (IsDebuggerPresent())
    {
        DebugSettings->EnableFrameRateCounter = true;
    }
#endif
    auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

    if (rootFrame == nullptr)
    {
        rootFrame = ref new Frame();

        rootFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);

        if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
        {
            // TODO: Restore the saved session state only when appropriate, scheduling the
            // final launch steps after the restore is complete
        }

        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                if (jsAppStarted_) {
                    rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
                }
                else {
                    auto appStartEvt = ConvertStdStrToPlatform(voicemynews::core::events::kAppJsStart);
                    jsApp_->GetEventLoop()->On(appStartEvt,
                        ref new voicemynews::app::win10::bindings::events::EventHandler([rootFrame, e](EventDataBinding^ evtData) {
                        concurrency::create_task(rootFrame->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([rootFrame, e] {
                            rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
                        })));
                    }));
                }
            }

            Window::Current->Content = rootFrame;
            Window::Current->Activate();
        }
    }
    else
    {
        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }

            Window::Current->Activate();
        }
    }
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
    (void)sender;  // Unused parameter
    (void)e;   // Unused parameter

                //TODO: Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e)
{
    throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}
}
