#include "pch.h"
#include "CppUnitTest.h"
#include "events/EventNames.h"
#include "utils/Conversions.h"

#include <ppltasks.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Platform::String;
using voicemynews::app::win10::components::CategoriesPreferences;
using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::bindings::events::EventHandler;
using voicemynews::app::win10::bindings::events::EventLoopBinding;
using voicemynews::app::win10::js::JsApp;
using voicemynews::core::events::kCategoriesGet;
using voicemynews::core::events::kCategoriesGetLoaded;
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
namespace components {
/**
 * \brief This class provides the unit tests which guarantees correct functionality of categories preferences component.
 */
TEST_CLASS(CategoriesPreferencesTests) {
public:
    TEST_METHOD_INITIALIZE(CategoriesPreferencesTestsSetUp) {
        receivedGetCategories_ = false;
        jsLoop_ = ref new EventLoopBinding();
        jsBackend_ = ref new JsApp(jsLoop_);
        dispatcher_ = CoreApplication::CreateNewView()->Dispatcher;

        jsLoop_->On(ConvertStdStrToPlatform(kCategoriesGet),
            ref new EventHandler([this](EventDataBinding^ evtData) {
            receivedGetCategories_ = true;
        }));

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::High,
            ref new DispatchedHandler([this]() {
            categoriesPrefs_ = ref new CategoriesPreferences();
            categoriesPrefs_->JsBackend = jsBackend_;
        }))).then([this]() {
            jsLoop_->ProcessEvents();
        }).wait();

        Assert::IsTrue(receivedGetCategories_);
    }

    TEST_METHOD(CategoriesPreferencesTestsLoadedCategoriesOk) {
        auto categoriesData = ref new JsonArray();
        auto categ1 = ref new JsonObject();
        categ1->SetNamedValue("categoryId", JsonValue::CreateNumberValue(1));
        categ1->SetNamedValue("name", JsonValue::CreateStringValue("category 1"));
        categ1->SetNamedValue("icon", JsonValue::CreateStringValue("/path/to/my/icon/category1.png"));

        auto categ2 = ref new JsonObject();
        categ2->SetNamedValue("categoryId", JsonValue::CreateNumberValue(2));
        categ2->SetNamedValue("name", JsonValue::CreateStringValue("category 2"));
        categ2->SetNamedValue("icon", JsonValue::CreateStringValue("/path/to/my/icon/category2.png"));

        categoriesData->Append(categ1);
        categoriesData->Append(categ2);

        IVector<IJsonObject^>^ initialCategories = nullptr;
        IVector<IJsonObject^>^ loadedCategories = nullptr;

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::High,
            ref new DispatchedHandler([&initialCategories, this]() {
            initialCategories = categoriesPrefs_->Categories;
        }))).then([categoriesData, &loadedCategories, this]() {
            jsLoop_->Emit(ConvertStdStrToPlatform(kCategoriesGetLoaded),
                ref new EventDataBinding(categoriesData->ToString()));
            jsLoop_->ProcessEvents();
        }).wait();

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::High,
            ref new DispatchedHandler([&loadedCategories, this]() {
            loadedCategories = categoriesPrefs_->Categories;
        }))).wait();

        Assert::IsNull(initialCategories);
        Assert::IsNotNull(loadedCategories);
        Assert::AreEqual(categoriesData->Size, loadedCategories->Size);

        unsigned int idx = 0;
        for (auto it = loadedCategories->First(); it->HasCurrent; it->MoveNext()) {
            auto expectedJson = categoriesData->GetAt(idx++)->ToString();
            auto actualJson = it->Current->ToString();
            Assert::AreEqual(expectedJson, actualJson);
        }
    }

private:
    bool receivedGetCategories_;
    CoreDispatcher^ dispatcher_;
    EventLoopBinding^ jsLoop_;
    JsApp^ jsBackend_;
    CategoriesPreferences^ categoriesPrefs_;
};
}
}
}
}
}