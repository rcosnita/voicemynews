#include "pch.h"
#include "CppUnitTest.h"

#include <ppltasks.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Platform::String;
using voicemynews::app::win10::components::MainMenuItem;
using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::bindings::events::EventHandler;
using voicemynews::app::win10::bindings::events::EventLoopBinding;
using voicemynews::app::win10::js::JsApp;
using Windows::ApplicationModel::Core::CoreApplication;
using Windows::Data::Json::JsonObject;
using Windows::UI::Core::CoreDispatcher;
using Windows::UI::Core::CoreDispatcherPriority;
using Windows::UI::Core::DispatchedHandler;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
namespace components {
/**
 * \brief Provides the tests suite for making sure that menu item component is correctly implemented.
 */
TEST_CLASS(MainMenuItemTests) {
public:
    TEST_METHOD_INITIALIZE(MainMenuItemTestSetUp) {
        dispatcher_ = CoreApplication::CreateNewView()->Dispatcher;

        jsBackend_ = ref new JsApp();
        jsLoop_ = jsBackend_->GetEventLoop();

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this] {
            menuItem_ = ref new MainMenuItem();
            menuItem_->JsBackend = jsBackend_;
        }))).wait();
    }

    TEST_METHOD(MainMenuItemTestsItemClickOk) {
        String^ modelStr = "{\"icon\":\"/Assets/icons/menuitems/preferences.png\",\"label\":\"Preferences\",\"type\":\"item\",";
        modelStr += "\"data\":{\"evtName\":\"js:menuitems:openPreferences\"}}";

        auto model = JsonObject::Parse(modelStr);
        bool eventTriggered = false;
        String^ receivedEvtData = nullptr;

        jsLoop_->On("js:menuitems:openPreferences", ref new EventHandler([&eventTriggered, &receivedEvtData](EventDataBinding^ evtData) {
            eventTriggered = true;

            Assert::IsNotNull(evtData);
            receivedEvtData = evtData->EvtData;
        }));

        concurrency::create_task(dispatcher_->RunAsync(CoreDispatcherPriority::Normal,
            ref new DispatchedHandler([this, &model] {
            menuItem_->DataContext = model;
            menuItem_->OnMenuItemClicked(menuItem_, nullptr);
        }))).then([&eventTriggered, &receivedEvtData, &modelStr, this]() {
            jsLoop_->ProcessEvents();
        }).wait();

        Assert::IsTrue(eventTriggered);
        Assert::IsNotNull(receivedEvtData);
        Assert::AreEqual(modelStr, receivedEvtData);
    }

private:
    CoreDispatcher^ dispatcher_;
    EventLoopBinding^ jsLoop_;
    JsApp^ jsBackend_;
    MainMenuItem^ menuItem_;
};
}
}
}
}
}