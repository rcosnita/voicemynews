#include "pch.h"
#include "pages/UserPreferencesPage.xaml.h"
#include "JsApp.h"

using namespace voicemynews;

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

using voicemynews::app::win10::js::JsApp;

static DependencyProperty^ JsBackendProperty = DependencyProperty::Register(
    "JsBackend",
    JsApp::typeid,
    voicemynews::app::win10::pages::UserPreferencesPage::typeid,
    ref new PropertyMetadata(nullptr)
);

namespace voicemynews {
namespace app {
namespace win10 {
namespace pages {
JsApp^ UserPreferencesPage::JsBackend::get() {
    return static_cast<JsApp^>(GetValue(JsBackendProperty));
}

void UserPreferencesPage::JsBackend::set(JsApp^ value) {
    SetValue(JsBackendProperty, value);
}

UserPreferencesPage::UserPreferencesPage()
{
    InitializeComponent();

    JsBackend = JsApp::GetInstance();

    DataContext = this;
}
}
}
}
}
