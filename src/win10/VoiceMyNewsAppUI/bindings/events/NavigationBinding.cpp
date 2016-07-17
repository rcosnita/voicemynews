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

static voicemynews::app::win10::bindings::events::NavigationBinding^ navigationInstance = nullptr;

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
NavigationBinding::NavigationBinding() {
    menuNavigationMapping_ = ref new Platform::Collections::Map<String^, TypeName>();

    menuNavigationMapping_->Insert(ConvertStdStrToPlatform(voicemynews::core::events::kMenuItemOpenGeniusNews),
        voicemynews::app::win10::pages::GeniusNewsPage::typeid);
    menuNavigationMapping_->Insert(ConvertStdStrToPlatform(voicemynews::core::events::kMenuItemOpenPreferences),
        voicemynews::app::win10::pages::UserPreferencesPage::typeid);
}

void NavigationBinding::MenuContentView::set(Frame^ menuContentView) {
    menuContentView_ = menuContentView;
}

Frame^ NavigationBinding::MenuContentView::get() {
    return menuContentView_;
}

NavigationBinding^ NavigationBinding::GetInstance() {
    if (navigationInstance == nullptr) {
        navigationInstance = ref new NavigationBinding();
    }

    return navigationInstance;
}

void NavigationBinding::NavigateByEvent(String^ evtName, EventDataBinding^ evtData) {
    if (menuNavigationMapping_->HasKey(evtName)) {
        concurrency::create_task(menuContentView_->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, evtName, evtData]() {
            menuContentView_->Navigate(menuNavigationMapping_->Lookup(evtName), evtData);
        })));
    }
}
}
}
}
}
}