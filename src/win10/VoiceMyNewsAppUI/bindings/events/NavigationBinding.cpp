#include "pch.h"
#include "NavigationBinding.h"
#include "pages/GeniusNewsPage.xaml.h"
#include "pages/UserPreferencesPage.xaml.h"
#include "events/EventNames.h"
#include "utils/Conversions.h"

#include <ppltasks.h>

using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using Windows::UI::Core::CoreDispatcherPriority;
using Windows::UI::Core::DispatchedHandler;
using Windows::UI::Xaml::Controls::Frame;
using Windows::UI::Xaml::Interop::TypeName;

static voicemynews::app::win10::bindings::events::NavigationBinding^ navigationInstance = nullptr;

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
FrameAppNavigator::FrameAppNavigator(Frame^ frame) {
    frame_ = frame;
}

bool FrameAppNavigator::Navigate(TypeName pageType, EventDataBinding^ navigationData) {
    bool isSuccess = false;
    concurrency::create_task(frame_->Dispatcher->RunAsync(CoreDispatcherPriority::High,
        ref new DispatchedHandler([this, &pageType, &navigationData, &isSuccess]() {
        isSuccess = frame_->Navigate(pageType, navigationData);
    }))).wait();

    return isSuccess;
}

NavigationBinding::NavigationBinding() {
    InitComponent();
}

NavigationBinding::NavigationBinding(IAppNavigator^ menuContentNavigator)
    : menuContentNavigator_(menuContentNavigator) {
    InitComponent();
}

void NavigationBinding::InitComponent() {
    menuNavigationMapping_ = ref new Platform::Collections::Map<String^, TypeName>();

    menuNavigationMapping_->Insert(ConvertStdStrToPlatform(voicemynews::core::events::kMenuItemOpenGeniusNews),
        voicemynews::app::win10::pages::GeniusNewsPage::typeid);
    menuNavigationMapping_->Insert(ConvertStdStrToPlatform(voicemynews::core::events::kMenuItemOpenPreferences),
        voicemynews::app::win10::pages::UserPreferencesPage::typeid);
}

void NavigationBinding::MenuContentNavigator::set(IAppNavigator^ menuContentNavigator) {
    menuContentNavigator_ = menuContentNavigator;
}

IAppNavigator^ NavigationBinding::MenuContentNavigator::get() {
    return menuContentNavigator_;
}

NavigationBinding^ NavigationBinding::GetInstance() {
    if (navigationInstance == nullptr) {
        navigationInstance = ref new NavigationBinding();
    }

    return navigationInstance;
}

bool NavigationBinding::NavigateByEvent(String^ evtName, EventDataBinding^ evtData) {
    if (menuNavigationMapping_->HasKey(evtName)) {
        return MenuContentNavigator->Navigate(menuNavigationMapping_->Lookup(evtName), evtData);
    }

    return false;
}
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace events {
using NavigationBinding = voicemynews::app::win10::bindings::events::NavigationBinding;
NavigationBinding^ NavigationManagerPlatform::GetInstance() {
    return NavigationBinding::GetInstance();
}
}
}
}