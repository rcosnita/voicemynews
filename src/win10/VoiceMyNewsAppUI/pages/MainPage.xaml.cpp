//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "components/MainMenu.xaml.h"

namespace voicemynews {
namespace app {
namespace win10 {
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

using JsApp = voicemynews::app::win10::js::JsApp;
using NavigationBinding = voicemynews::app::win10::bindings::events::NavigationBinding;

static DependencyProperty^ IsMenuVisibleProperty = DependencyProperty::RegisterAttached(
    "IsMenuVisible",
    Interop::TypeName(bool::typeid),
    Interop::TypeName(MainPage::typeid),
    ref new PropertyMetadata(nullptr)
);

static DependencyProperty^ JsBackendProperty = DependencyProperty::RegisterAttached(
    "JsBackend",
    Interop::TypeName(JsApp::typeid),
    Interop::TypeName(MainPage::typeid),
    ref new PropertyMetadata(nullptr)
);

bool MainPage::IsMenuVisible::get() {
    return static_cast<bool>(GetValue(IsMenuVisibleProperty));
}

void MainPage::IsMenuVisible::set(bool value) {
    SetValue(IsMenuVisibleProperty, value);
}

JsApp^ MainPage::JsBackend::get() {
    return static_cast<JsApp^>(GetValue(JsBackendProperty));
}

void MainPage::JsBackend::set(JsApp^ value) {
    SetValue(JsBackendProperty, value);
}

MainPage::MainPage()
{
    InitializeComponent();
    JsBackend = JsApp::GetInstance();

    navigationBinding_ = NavigationBinding::GetInstance();
    navigationBinding_->MenuContentView = FrameAppActiveContent;

    DataContext = this;
}

void MainPage::OnMenuExpanded(bool isMenuExpanded) {
    IsMenuVisible = isMenuExpanded;
}
}
}
}