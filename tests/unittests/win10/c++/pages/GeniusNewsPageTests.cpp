#include "pch.h"
#include "CppUnitTest.h"
#include "events/EventNames.h"
#include "utils/Conversions.h"

#include <ppltasks.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Platform::String;
using voicemynews::app::win10::pages::GeniusNewsPage;
using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::bindings::events::EventHandler;
using voicemynews::app::win10::bindings::events::EventLoopBinding;
using voicemynews::app::win10::js::JsApp;
using voicemynews::core::events::kNewsFetchFromPreferredCategories;
using voicemynews::core::events::kNewsFetchFromPreferredCategoriesLoaded;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using Windows::ApplicationModel::Core::CoreApplication;
using Windows::Data::Json::IJsonObject;
using Windows::Data::Json::JsonArray;
using Windows::Data::Json::JsonObject;
using Windows::Data::Json::JsonValue;
using Windows::Foundation::Collections::IVector;
using Windows::UI::Core::CoreDispatcher;
using Windows::UI::Core::CoreDispatcherPriority;
using Windows::UI::Core::DispatchedHandler;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
namespace pages {
/**
 * \brief Provides all tests which ensure correct page functionality.
 */
TEST_CLASS(GeniusNewsPageTests) {
public:
    TEST_METHOD_INITIALIZE(GeniusNewsPageTestsSetUp) {
        receivedNews_ = false;
        jsBackend_ = ref new JsApp();
        jsLoop_ = jsBackend_->GetEventLoop();
        dispatcher_ = CoreApplication::CreateNewView()->Dispatcher;

        jsLoop_->On(ConvertStdStrToPlatform(kNewsFetchFromPreferredCategoriesLoaded),
            ref new EventHandler([this](EventDataBinding^ evtData) {
            receivedNews_ = true;
        }));

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::High,
            ref new DispatchedHandler([this]() {
            geniusPage_ = ref new GeniusNewsPage(jsBackend_);
        }))).then([this]() {
            jsLoop_->ProcessEvents();
        }).wait();

        Assert::IsFalse(receivedNews_);
    }

    TEST_METHOD(GeniusNewsPageTestsLoadedOk) {
        auto newsData = ref new JsonArray();
        auto news1 = ref new JsonObject();
        auto news2 = ref new JsonObject();
        
        news1->SetNamedValue("headline", JsonValue::CreateStringValue("sample article"));
        news1->SetNamedValue("images", ref new JsonObject());
        news2->SetNamedValue("headline", JsonValue::CreateStringValue("sample article 2"));
        news2->SetNamedValue("images", JsonObject::Parse("{\"small\": {\"url\":\"http://sample.com/img.jpg\",\"width\":300,\"height\":300}}"));

        newsData->Append(news1);
        newsData->Append(news2);

        TestDisplayNewsTemplate(newsData);
    }

    TEST_METHOD(GeniusNewsPageTestsEmptyDataOk) {
        TestDisplayNewsTemplate(ref new JsonArray());
    }

private:
    /**
     * \brief This method provides a template which allows us to correctly test the display news action.
     */
    void TestDisplayNewsTemplate(JsonArray^ expectedData) {
        auto evtData = ref new EventDataBinding(expectedData->ToString());
        jsLoop_->Emit(ConvertStdStrToPlatform(kNewsFetchFromPreferredCategoriesLoaded), evtData);
        jsLoop_->ProcessEvents();

        bool newsOk = false;

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::High,
            ref new DispatchedHandler([&newsOk, expectedData, this]() {
            auto fetchedCategories = geniusPage_->News;

            newsOk = fetchedCategories->Size == expectedData->Size;
            int idx = 0;

            for (auto it = fetchedCategories->First(); it->HasCurrent; it->MoveNext()) {
                fetchedCategories->GetAt(idx)->ToString() == fetchedCategories->GetAt(idx)->ToString() &&
                idx++;
            }
        }))).wait();

        Assert::IsTrue(receivedNews_);
        Assert::IsTrue(newsOk);
    }

private:
    bool receivedNews_;
    CoreDispatcher^ dispatcher_;
    EventLoopBinding^ jsLoop_;
    JsApp^ jsBackend_;
    GeniusNewsPage^ geniusPage_;
};
}
}
}
}
}