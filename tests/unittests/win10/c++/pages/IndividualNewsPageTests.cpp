#include "pch.h"
#include "CppUnitTest.h"
#include "events/EventNames.h"
#include "utils/Conversions.h"

#include <chrono>
#include <mutex>
#include <ppltasks.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Platform::String;
using voicemynews::app::win10::pages::IndividualNewsPage;
using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::bindings::events::EventHandler;
using voicemynews::app::win10::bindings::events::EventLoopBinding;
using voicemynews::app::win10::js::JsApp;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using voicemynews::core::events::kNewsFetchByUrlLoaded;
using voicemynews::core::events::kNewsVoiceRead;

using Windows::ApplicationModel::Core::CoreApplication;
using Windows::Data::Json::JsonArray;
using Windows::Data::Json::IJsonObject;
using Windows::Data::Json::JsonObject;
using Windows::Data::Json::JsonValue;
using Windows::Foundation::Collections::IVector;
using Windows::UI::Core::CoreDispatcher;
using Windows::UI::Core::CoreDispatcherPriority;
using Windows::UI::Core::DispatchedHandler;
using Windows::UI::Xaml::Controls::Control;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
namespace pages {
/**
 * \brief This class provides the tests suite which guarantees individual news are correctly displayed.
 */
TEST_CLASS(IndividualNewsPageTests) {
public:
    TEST_METHOD_INITIALIZE(IndividualNewsPageTestsSetUp)
    {
        jsLoop_ = ref new EventLoopBinding(true);
        jsBackend_ = ref new JsApp(jsLoop_);
        dispatcher_ = CoreApplication::CreateNewView()->Dispatcher;

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::High,
            ref new DispatchedHandler([this]() {
            newsPage_ = ref new IndividualNewsPage(jsBackend_);
        }))).wait();

        Assert::IsNotNull(newsPage_);
    }

    TEST_METHOD(IndividualNewsPageTestsDisplayEmptyNewsOk)
    {
        auto newsModel = ref new JsonObject();
        newsModel->SetNamedValue("headline", JsonValue::CreateStringValue("sample article"));
        newsModel->SetNamedValue("paragraphs", JsonValue::Parse("[]"));
        newsModel->SetNamedValue("contributedBy", JsonValue::Parse("[]"));

        jsLoop_->Emit(ConvertStdStrToPlatform(kNewsFetchByUrlLoaded),
            ref new EventDataBinding(newsModel->ToString()));

        IJsonObject^ actualNewsModel = nullptr;
        IVector<IJsonObject^>^ actualParagrahsModel = nullptr;
        IVector<String^>^ actualContributedByModel = nullptr;

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::High,
            ref new DispatchedHandler([this, &actualNewsModel, &actualParagrahsModel, &actualContributedByModel]() {
            actualNewsModel = newsPage_->CurrNews;
            actualParagrahsModel = newsPage_->Paragraphs;
            actualContributedByModel = newsPage_->ContributedBy;
        }))).wait();

        Assert::IsNotNull(actualNewsModel);
        Assert::AreEqual(actualNewsModel->ToString(), newsModel->ToString());
        Assert::IsNotNull(actualParagrahsModel);
        Assert::IsTrue(actualParagrahsModel->Size == 0);
        Assert::IsNotNull(actualContributedByModel);
        Assert::IsTrue(actualContributedByModel->Size == 0);
    }

    TEST_METHOD(IndividualNewsPageTestsDisplayNewsOk)
    {
        auto newsModel = ref new JsonObject();
        newsModel->SetNamedValue("headline", JsonValue::CreateStringValue("sample article"));
        newsModel->SetNamedValue("paragraphs", JsonValue::Parse("[{\"content\":\"p1\"}, {\"content\":\"p2\"}]"));
        newsModel->SetNamedValue("contributedBy", JsonValue::Parse("[\"Contributor 1\", \"Contributor 2\"]"));

        jsLoop_->Emit(ConvertStdStrToPlatform(kNewsFetchByUrlLoaded),
            ref new EventDataBinding(newsModel->ToString()));

        IJsonObject^ actualNewsModel = nullptr;
        IVector<IJsonObject^>^ actualParagrahsModel = nullptr;
        IVector<String^>^ actualContributedByModel = nullptr;

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::High,
            ref new DispatchedHandler([this, &actualNewsModel, &actualParagrahsModel, &actualContributedByModel]() {
            actualNewsModel = newsPage_->CurrNews;
            actualParagrahsModel = newsPage_->Paragraphs;
            actualContributedByModel = newsPage_->ContributedBy;
        }))).wait();

        Assert::IsNotNull(actualNewsModel);
        Assert::AreEqual(actualNewsModel->ToString(), newsModel->ToString());
        
        Assert::IsNotNull(actualParagrahsModel);
        Assert::IsTrue(actualParagrahsModel->Size == 2);
        auto idx = 0;
        for (auto it = actualParagrahsModel->First(); it->HasCurrent; it->MoveNext()) {
            Assert::AreEqual(it->Current->ToString(), actualParagrahsModel->GetAt(idx++)->ToString());
        }

        Assert::IsNotNull(actualContributedByModel);
        Assert::IsTrue(actualContributedByModel->Size == 2);
        idx = 0;
        for (auto it = actualContributedByModel->First(); it->HasCurrent; it->MoveNext()) {
            Assert::AreEqual(it->Current->ToString(), actualContributedByModel->GetAt(idx++)->ToString());
        }
    }

    TEST_METHOD(IndividualNewsPageTestsReadOk)
    {
        std::mutex eventHandlerMutex;
        std::unique_lock<std::mutex> eventHandlerLock(eventHandlerMutex);
        std::condition_variable eventHandlerNotifier;
        bool eventReceived = false;

        String^ receivedNews = nullptr;
        bool btnReadEnabled = true;

        auto newsModel = ref new JsonObject();
        newsModel->SetNamedValue("headline", JsonValue::CreateStringValue("sample article"));
        newsModel->SetNamedValue("paragraphs", JsonValue::Parse("[]"));
        newsModel->SetNamedValue("contributedBy", JsonValue::Parse("[]"));

        jsLoop_->On(ConvertStdStrToPlatform(kNewsVoiceRead),
            ref new EventHandler([this, &eventHandlerMutex, &eventHandlerNotifier, &eventReceived,
                &receivedNews](EventDataBinding^ evt) {
            std::lock_guard<std::mutex> lk(eventHandlerMutex);
            receivedNews = evt->EvtData;
            eventReceived = true;
            eventHandlerNotifier.notify_all();
        }));

        jsLoop_->Emit(ConvertStdStrToPlatform(kNewsFetchByUrlLoaded),
            ref new EventDataBinding(newsModel->ToString()));

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::High,
            ref new DispatchedHandler([this, &btnReadEnabled, newsModel]() {
            newsPage_->ReadNews();
            btnReadEnabled = safe_cast<Control^>(newsPage_->FindName("btnReadNews"))->IsEnabled;
        }))).wait();

        eventHandlerNotifier.wait_for(eventHandlerLock, std::chrono::milliseconds{20000},
            [&eventReceived] { return eventReceived; });

        Assert::IsNotNull(receivedNews);
        Assert::AreEqual(newsModel->ToString(), receivedNews);
        Assert::IsFalse(btnReadEnabled);
    }

private:
    EventLoopBinding^ jsLoop_;
    JsApp^ jsBackend_;
    IndividualNewsPage^ newsPage_;
    CoreDispatcher^ dispatcher_;
};
}
}
}
}
}