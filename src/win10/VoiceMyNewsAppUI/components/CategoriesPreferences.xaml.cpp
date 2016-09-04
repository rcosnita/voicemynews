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
    ref new PropertyMetadata(nullptr, 
        ref new PropertyChangedCallback(&voicemynews::app::win10::components::CategoriesPreferences::OnJsBackendChanged))
);

namespace voicemynews {
namespace app {
namespace win10 {
namespace components {
CategoriesPreferences::CategoriesPreferences()
{
    InitControl(nullptr);
}

CategoriesPreferences::CategoriesPreferences(JsApp^ jsBackend)
{
    InitControl(jsBackend);
}

void CategoriesPreferences::InitControl(JsApp^ jsBackend)
{
    InitializeComponent();

    if (jsBackend != nullptr) {
        JsBackend = jsBackend;
    }

    DataContext = this;

    Unloaded += ref new Windows::UI::Xaml::RoutedEventHandler(this, &CategoriesPreferences::OnUnloaded);
}

JsApp^ CategoriesPreferences::JsBackend::get()
{
    return static_cast<JsApp^>(GetValue(JsBackendProperty));
}

void CategoriesPreferences::JsBackend::set(JsApp^ jsBackend)
{
    SetValue(JsBackendProperty, jsBackend);
}

void CategoriesPreferences::OnJsBackendChanged(DependencyObject^ d, DependencyPropertyChangedEventArgs^ args)
{
    auto comp = safe_cast<CategoriesPreferences^>(d);
    comp->WireEvents();
}

IVector<IJsonObject^>^ CategoriesPreferences::Categories::get()
{
    return static_cast<IVector<IJsonObject^>^>(GetValue(CategoriesProperty));
}

void CategoriesPreferences::Categories::set(IVector<IJsonObject^>^ categories)
{
    SetValue(CategoriesProperty, categories);
}

void CategoriesPreferences::WireEvents()
{
    auto jsEventLoop = JsBackend->GetEventLoop();

    concurrency::create_async([this, jsEventLoop]() {
        onCategoriesLoadedId = jsEventLoop->On(ConvertStdStrToPlatform(kCategoriesGetLoaded),
            ref new voicemynews::app::win10::bindings::events::EventHandler(this, &CategoriesPreferences::OnCategoriesLoaded));

        jsEventLoop->Emit(ConvertStdStrToPlatform(kCategoriesGet), ref new EventDataBinding(""));
    });
}

void CategoriesPreferences::OnCategoriesLoaded(EventDataBinding^ evtData)
{
    auto categoriesData = JsonArray::Parse(evtData->EvtData);

    concurrency::create_task(Dispatcher->RunAsync(CoreDispatcherPriority::High,
        ref new DispatchedHandler([categoriesData, this] {
        Categories = ConvertJsonArrayToVector(*categoriesData);
    }))).wait();
}

void CategoriesPreferences::OnUnloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    auto jsEventLoop = JsBackend->GetEventLoop();

    concurrency::create_async([this, jsEventLoop]() {
        jsEventLoop->Off(onCategoriesLoadedId);
    });
}

}
}
}
}