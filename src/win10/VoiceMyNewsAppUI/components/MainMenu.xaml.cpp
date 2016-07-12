//
// MainMenu.xaml.cpp
// Implementation of the MainMenu class
//

#include "pch.h"
#include "MainMenu.xaml.h"
#include "pages/UserPreferencesPage.xaml.h"
#include "events/EventNames.h"
#include "utils/Conversions.h"

#include <ppltasks.h>

namespace voicemynews {
namespace app {
namespace win10 {
namespace components {
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using Platform::Collections::Vector;
using voicemynews::app::win10::bindings::events::EventHandler;
using voicemynews::app::win10::pages::UserPreferencesPage;
using voicemynews::app::win10::utils::ConvertJsonArrayToVector;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using Windows::Data::Json::JsonObject;
using Windows::UI::Core::CoreDispatcherPriority;
using Windows::UI::Core::DispatchedHandler;

static DependencyProperty^ IsMenuExpandedProperty = DependencyProperty::RegisterAttached(
    "IsMenuExpanded",
    bool::typeid,
    MainMenu::typeid,
    ref new PropertyMetadata(nullptr)
);

static DependencyProperty^ JsBackendProperty = DependencyProperty::RegisterAttached(
    "JsBackend",
    JsApp::typeid,
    MainMenu::typeid,
    ref new PropertyMetadata(nullptr)
);

static DependencyProperty^ MenuItemsProperty = DependencyProperty::RegisterAttached(
    "MenuItems",
    JsonArray::typeid,
    MainMenu::typeid,
    ref new PropertyMetadata(nullptr)
);

bool MainMenu::IsMenuExpanded::get() {
    return static_cast<bool>(GetValue(IsMenuExpandedProperty));
}

void MainMenu::IsMenuExpanded::set(bool value) {
    SetValue(IsMenuExpandedProperty, value);

    MenuExpanded(value);
}

JsApp^ MainMenu::JsBackend::get() {
    return static_cast<JsApp^>(GetValue(JsBackendProperty));
}

void MainMenu::JsBackend::set(JsApp^ value) {
    SetValue(JsBackendProperty, value);
}

IVector<IJsonObject^>^ MainMenu::MenuItems::get() {
    return static_cast<IVector<IJsonObject^>^>(GetValue(MenuItemsProperty));
}

void MainMenu::MenuItems::set(IVector<IJsonObject^>^ value) {
    SetValue(MenuItemsProperty, value);
}

Frame^ MainMenu::ContentView::get() {
    return contentView_;
}

void MainMenu::ContentView::set(Frame^ value) {
    contentView_ = value;
}

MainMenu::MainMenu()
{
    InitializeComponent();

    WireJsMenuModel();
    WireJsNavigationEvents();

    DataContext = this;
}

void MainMenu::OpenMenu(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    auto jsBackend = JsBackend;
    IsMenuExpanded = !IsMenuExpanded;
}

void MainMenu::WireJsMenuModel() {
    auto eventLoop = JsBackend->GetEventLoop();
    
    eventLoop->On(ConvertStdStrToPlatform(voicemynews::core::events::kAppNavigationMenuLoaded),
        ref new EventHandler([this](EventDataBinding^ evtData) {
        OnMenuLoaded(evtData);
    }));

    eventLoop->Emit(ConvertStdStrToPlatform(voicemynews::core::events::kAppNavigationMenuLoad),
        ref new EventDataBinding(""));
}

void MainMenu::WireJsNavigationEvents() {
    auto eventLoop = JsBackend->GetEventLoop();

    eventLoop->On(ConvertStdStrToPlatform(voicemynews::core::events::kMenuItemOpenPreferences),
        ref new EventHandler([this](EventDataBinding^ evtData) {
        concurrency::create_task(Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this]() {
            ContentView->Navigate(UserPreferencesPage::typeid);
        })));
    }));
}

void MainMenu::OnMenuLoaded(EventDataBinding^ evtData) {
    concurrency::create_task(Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this, evtData]() {
        auto model = JsonArray::Parse(evtData->EvtData);
        MenuItems = ConvertJsonArrayToVector(*model);
    })));
}

}
}
}
}
