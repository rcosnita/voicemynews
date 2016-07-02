//
// MainMenu.xaml.cpp
// Implementation of the MainMenu class
//

#include "pch.h"
#include "MainMenu.xaml.h"

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

static DependencyProperty^ IsMenuExpandedProperty = DependencyProperty::RegisterAttached(
    "IsMenuExpanded",
    Interop::TypeName(bool::typeid),
    Interop::TypeName(MainMenu::typeid),
    ref new PropertyMetadata(nullptr)
);

static DependencyProperty^ JsBackendProperty = DependencyProperty::RegisterAttached(
    "JsBackend",
    Interop::TypeName(JsApp::typeid),
    Interop::TypeName(MainMenu::typeid),
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

MainMenu::MainMenu()
{
    InitializeComponent();
}

void MainMenu::OpenMenu(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    auto jsBackend = JsBackend;
    IsMenuExpanded = !IsMenuExpanded;
}

}
}
}
}
