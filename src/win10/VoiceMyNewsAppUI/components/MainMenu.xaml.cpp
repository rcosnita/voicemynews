//
// MainMenu.xaml.cpp
// Implementation of the MainMenu class
//

#include "pch.h"
#include "MainMenu.xaml.h"
#include "events/EventNames.h"
#include "pages/UserPreferencesPage.xaml.h"
#include "pages/GeniusNewsPage.xaml.h"
#include "utils/Conversions.h"

#include <ppltasks.h>

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
using voicemynews::app::win10::js::JsApp;
using voicemynews::app::win10::pages::GeniusNewsPage;
using voicemynews::app::win10::pages::UserPreferencesPage;
using voicemynews::app::win10::utils::ConvertJsonArrayToVector;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using Windows::Data::Json::IJsonObject;
using Windows::Data::Json::JsonArray;
using Windows::Data::Json::JsonObject;
using Windows::UI::Core::CoreDispatcherPriority;
using Windows::UI::Core::DispatchedHandler;

namespace voicemynews {
namespace app {
namespace win10 {
namespace components {
using EventHandler = voicemynews::app::win10::bindings::events::EventHandler;
static DependencyProperty^ IsMenuExpandedProperty = DependencyProperty::Register(
    "IsMenuExpanded",
    bool::typeid,
    MainMenu::typeid,
    ref new PropertyMetadata(nullptr)
);

static DependencyProperty^ JsBackendProperty = DependencyProperty::Register(
    "JsBackend",
    JsApp::typeid,
    MainMenu::typeid,
    ref new PropertyMetadata(nullptr,
        ref new PropertyChangedCallback(&MainMenu::OnJsBackendChanged))
);

static DependencyProperty^ MenuItemsProperty = DependencyProperty::Register(
    "MenuItems",
    JsonArray::typeid,
    MainMenu::typeid,
    ref new PropertyMetadata(nullptr)
);

MainMenu::MainMenu()
{
    InitializeComponent();
    DataContext = this;
}

MainMenu::MainMenu(JsApp^ jsApp) {
    InitializeComponent();
    JsBackend = jsApp;
    DataContext = this;
}

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

void MainMenu::OnJsBackendChanged(DependencyObject^ d, DependencyPropertyChangedEventArgs^ args) {
    auto comp = safe_cast<MainMenu^>(d);
    comp->WireJsMenuModel();
}

IVector<IJsonObject^>^ MainMenu::MenuItems::get() {
    return static_cast<IVector<IJsonObject^>^>(GetValue(MenuItemsProperty));
}

void MainMenu::MenuItems::set(IVector<IJsonObject^>^ value) {
    SetValue(MenuItemsProperty, value);
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

void MainMenu::OnMenuLoaded(EventDataBinding^ evtData) {
    concurrency::create_task(Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this, evtData]() {
        auto model = JsonArray::Parse(evtData->EvtData);
        MenuItems = ConvertJsonArrayToVector(*model);
    }))).wait();
}

}
}
}
}
