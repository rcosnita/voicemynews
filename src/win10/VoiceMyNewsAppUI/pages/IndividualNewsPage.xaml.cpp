//
// IndividualNewsPage.xaml.cpp
// Implementation of the IndividualNewsPage class
//

#include "pch.h"
#include "events/EventNames.h"
#include "utils/Conversions.h"
#include "IndividualNewsPage.xaml.h"

#include <ppltasks.h>

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
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
using voicemynews::app::win10::utils::ConvertJsonArrayToStrVector;
using voicemynews::app::win10::utils::ConvertJsonArrayToVector;

static DependencyProperty^ currNewsProperty = DependencyProperty::Register(
    "CurrNews",
    JsonObject::typeid,
    voicemynews::app::win10::pages::IndividualNewsPage::typeid,
    ref new PropertyMetadata(nullptr)
);

static DependencyProperty^ paragraphsProperty = DependencyProperty::Register(
    "Paragraphs",
    Vector<JsonObject^>::typeid,
    voicemynews::app::win10::pages::IndividualNewsPage::typeid,
    ref new PropertyMetadata(nullptr)
);

static DependencyProperty^ contributedByProperty = DependencyProperty::Register(
    "ContributedBy",
    Vector<JsonObject^>::typeid,
    voicemynews::app::win10::pages::IndividualNewsPage::typeid,
    ref new PropertyMetadata(nullptr)
);

namespace voicemynews {
namespace app {
namespace win10 {
namespace pages {
IndividualNewsPage::IndividualNewsPage()
{
    InitializeComponent();

    jsEventLoop_ = JsApp::GetInstance()->GetEventLoop();
    DataContext = this;
}

void IndividualNewsPage::CurrNews::set(IndividualNewsPage::NewsModel^ model)
{
    SetValue(currNewsProperty, model);
}

IndividualNewsPage::NewsModel^ IndividualNewsPage::CurrNews::get()
{
    return static_cast<IndividualNewsPage::NewsModel^>(GetValue(currNewsProperty));
}

void IndividualNewsPage::Paragraphs::set(IndividualNewsPage::ParagraphModel^ paragraphModel)
{
    SetValue(paragraphsProperty, paragraphModel);
}

IndividualNewsPage::ParagraphModel^ IndividualNewsPage::Paragraphs::get()
{
    return static_cast<IndividualNewsPage::ParagraphModel^>(GetValue(paragraphsProperty));
}

void IndividualNewsPage::ContributedBy::set(IndividualNewsPage::ContributedByModel^ contributedByModel)
{
    SetValue(contributedByProperty, contributedByModel);
}

IndividualNewsPage::ContributedByModel^ IndividualNewsPage::ContributedBy::get()
{
    return static_cast<IndividualNewsPage::ContributedByModel^>(GetValue(contributedByProperty));
}

void IndividualNewsPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs ^e)
{
    newsRssDesc_ = static_cast<IndividualNewsPage::NewsModel^>(e->Parameter);
    concurrency::create_task([this]() {
        onNewsLoadedId = jsEventLoop_->On(ConvertStdStrToPlatform(voicemynews::core::events::kNewsFetchByUrlLoaded),
            ref new voicemynews::app::win10::bindings::events::EventHandler([this](EventDataBinding^ evtData) {
            auto newsModelStr = evtData->EvtData;
            auto newsModel = JsonObject::Parse(newsModelStr);
            auto paragraphs = ConvertJsonArrayToVector(*(newsModel->GetNamedArray("paragraphs")));
            auto contributedBy = ConvertJsonArrayToStrVector(*(newsModel->GetNamedArray("contributedBy")));

            concurrency::create_task(Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal,
                ref new Windows::UI::Core::DispatchedHandler([this, contributedBy, newsModel, paragraphs]() {
                CurrNews = newsModel;
                Paragraphs = paragraphs;
                ContributedBy = contributedBy;
            })));
        }));

        jsEventLoop_->Emit(ConvertStdStrToPlatform(voicemynews::core::events::kNewsFetchByUrl),
            ref new EventDataBinding(newsRssDesc_->ToString()));
    });
}

void IndividualNewsPage::OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs ^e) {
    concurrency::create_task([this]() {
        jsEventLoop_->Off(onNewsLoadedId);
    });
}
}
}
}
}
