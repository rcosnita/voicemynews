#include "pch.h"
#include "MainMenuItem.xaml.h"

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

using Windows::Data::Json::IJsonObject;
using voicemynews::app::win10::js::JsApp;
using voicemynews::app::win10::bindings::events::EventDataBinding;

namespace voicemynews {
namespace app {
namespace win10 {
namespace components {
static DependencyProperty^ JsonModelProperty = DependencyProperty::RegisterAttached(
    "JsonModel",
    IJsonObject::typeid,
    MainMenuItem::typeid,
    ref new PropertyMetadata(nullptr));

static DependencyProperty^ JsBackendProperty = DependencyProperty::RegisterAttached(
    "JsBackend",
    JsApp::typeid,
    MainMenuItem::typeid,
    ref new PropertyMetadata(nullptr)
);

MainMenuItem::MainMenuItem()
{
    InitializeComponent();
}

JsApp^ MainMenuItem::JsBackend::get() {
    auto jsBackend = static_cast<JsApp^>(GetValue(JsBackendProperty));
    if (jsBackend == nullptr) {
        JsBackend = jsBackend = JsApp::GetInstance();
    }

    return jsBackend;
}

void MainMenuItem::JsBackend::set(JsApp^ value) {
    SetValue(JsBackendProperty, value);
}

void MainMenuItem::OnMenuItemClicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    auto jsonModel = static_cast<IJsonObject^>(DataContext);
    auto jsLoop = JsBackend->GetEventLoop();

    auto evtName = jsonModel->GetNamedObject("data")->GetNamedString("evtName");
    jsLoop->Emit(evtName, ref new EventDataBinding(jsonModel->ToString()));
}
}
}
}
}
