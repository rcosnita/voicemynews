#include "pch.h"
#include "bindings/events/EventDataBinding.h"
#include "bindings/events/EventLoopBinding.h"
#include "events/EventNames.h"
#include "utils/Conversions.h"
#include "GeniusNewsPage.xaml.h"
#include "IndividualNewsPage.xaml.h"

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

using Windows::Data::Json::IJsonArray;
using Windows::Data::Json::IJsonObject;
using Windows::Data::Json::JsonArray;
using voicemynews::app::win10::js::JsApp;
using NewsVector = Windows::Foundation::Collections::IVector<IJsonObject^>;

using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::utils::ConvertJsonArrayToVector;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using voicemynews::core::events::kNewsFetchFromPreferredCategories;
using voicemynews::core::events::kNewsFetchFromPreferredCategoriesLoaded;

static DependencyProperty^ NewsModelProperty = DependencyProperty::Register(
    L"News",
    NewsVector::typeid,
    voicemynews::app::win10::pages::GeniusNewsPage::typeid,
    ref new PropertyMetadata(nullptr)
);

static DependencyProperty^ JsBackendProperty = DependencyProperty::Register(
    "JsBackend",
    JsApp::typeid,
    voicemynews::app::win10::pages::GeniusNewsPage::typeid,
    ref new PropertyMetadata(nullptr, ref new PropertyChangedCallback(voicemynews::app::win10::pages::GeniusNewsPage::OnJsBackendChanged))
);

namespace voicemynews {
namespace app {
namespace win10 {
namespace pages {
GeniusNewsPage::GeniusNewsPage()
{
    InitPage(JsApp::GetInstance());
}

GeniusNewsPage::GeniusNewsPage(JsApp^ jsBackend) {
    InitPage(jsBackend);
}

void GeniusNewsPage::InitPage(JsApp^ jsBackend) {
    InitializeComponent();
    JsBackend = jsBackend;
    DataContext = this;
}

void GeniusNewsPage::OnJsBackendChanged(DependencyObject^ d, DependencyPropertyChangedEventArgs^ args) {
    auto page = safe_cast<GeniusNewsPage^>(d);
    page->WireJsModel();
}

JsApp^ GeniusNewsPage::JsBackend::get() {
    return static_cast<JsApp^>(GetValue(JsBackendProperty));
}

void GeniusNewsPage::JsBackend::set(JsApp^ jsBackend) {
    SetValue(JsBackendProperty, jsBackend);
}

NewsVector^ GeniusNewsPage::News::get() {
    return static_cast<NewsVector^>(GetValue(NewsModelProperty));
}

void GeniusNewsPage::News::set(NewsVector^ model) {
    SetValue(NewsModelProperty, model);
}

void GeniusNewsPage::WireJsModel() {
    auto jsLoop = JsBackend->GetEventLoop();

    concurrency::create_async([jsLoop, this]() {
        jsLoop->On(ConvertStdStrToPlatform(kNewsFetchFromPreferredCategoriesLoaded),
            ref new voicemynews::app::win10::bindings::events::EventHandler(this, &GeniusNewsPage::DisplayNews));

        jsLoop->Emit(ConvertStdStrToPlatform(kNewsFetchFromPreferredCategories), ref new EventDataBinding(""));
    });
}

void GeniusNewsPage::DisplayNews(EventDataBinding^ evtData) {
    auto newsArray = JsonArray::Parse(evtData->EvtData);

    concurrency::create_task(Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
        ref new Windows::UI::Core::DispatchedHandler([newsArray, this]() {
        News = ConvertJsonArrayToVector(*newsArray);
    }))).wait();
}

void GeniusNewsPage::NewsLst_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
    auto selectedItem = NewsLst->SelectedItem;
    Frame->Navigate(IndividualNewsPage::typeid, selectedItem);
}

}
}
}
}