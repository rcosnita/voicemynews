//
// CategoriesPreferences.xaml.cpp
// Implementation of the CategoriesPreferences class
//

#include "pch.h"
#include "events/EventNames.h"
#include "utils/Conversions.h"
#include "CategoriesPreferences.xaml.h"

#include <ppltasks.h>

using namespace voicemynews;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::js::JsApp;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using voicemynews::app::win10::utils::ConvertJsonArrayToVector;
using voicemynews::core::events::kCategoriesGet;
using voicemynews::core::events::kCategoriesGetLoaded;
using Windows::Data::Json::IJsonObject;
using Windows::Data::Json::JsonArray;
using Windows::Foundation::Collections::IVector;

static DependencyProperty^ CategoriesProperty = DependencyProperty::Register(
    "Categories",
    IVector<IJsonObject^>::typeid,
    voicemynews::app::win10::components::CategoriesPreferences::typeid,
    ref new PropertyMetadata(nullptr)
);

static DependencyProperty^ JsBackendProperty = DependencyProperty::Register(
    "JsBackend",
    JsApp::typeid,
    voicemynews::app::win10::components::CategoriesPreferences::typeid,
    ref new PropertyMetadata(nullptr)
);

namespace voicemynews {
namespace app {
namespace win10 {
namespace components {
CategoriesPreferences::CategoriesPreferences()
{
    InitializeComponent();

    WireEvents();

    DataContext = this;
}

CategoriesPreferences::CategoriesPreferences(JsApp^ jsBackend)
{
    InitializeComponent();
    JsBackend = jsBackend;

    WireEvents();

    DataContext = this;
}

JsApp^ CategoriesPreferences::JsBackend::get() {
    return static_cast<JsApp^>(GetValue(JsBackendProperty));
}

void CategoriesPreferences::JsBackend::set(JsApp^ jsBackend) {
    SetValue(JsBackendProperty, jsBackend);
}

IVector<IJsonObject^>^ CategoriesPreferences::Categories::get() {
    return static_cast<IVector<IJsonObject^>^>(GetValue(CategoriesProperty));
}

void CategoriesPreferences::Categories::set(IVector<IJsonObject^>^ categories) {
    SetValue(CategoriesProperty, categories);
}

void CategoriesPreferences::WireEvents() {
    auto jsEventLoop = JsBackend->GetEventLoop();

    concurrency::create_async([this, jsEventLoop]() {
        jsEventLoop->On(ConvertStdStrToPlatform(kCategoriesGetLoaded),
            ref new voicemynews::app::win10::bindings::events::EventHandler([this](EventDataBinding^ evtData) {
            OnCategoriesLoaded(evtData);
        }));

        jsEventLoop->Emit(ConvertStdStrToPlatform(kCategoriesGet), ref new EventDataBinding(""));
    });
}

void CategoriesPreferences::OnCategoriesLoaded(EventDataBinding^ evtData) {
    auto categoriesData = JsonArray::Parse(evtData->EvtData);

    concurrency::create_task(Dispatcher->RunAsync(CoreDispatcherPriority::High,
        ref new DispatchedHandler([categoriesData, this] {
        Categories = ConvertJsonArrayToVector(*categoriesData);
    }))).wait();
}
}
}
}
}