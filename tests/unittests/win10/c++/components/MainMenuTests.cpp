#include "pch.h"
#include "CppUnitTest.h"
#include "events/EventNames.h"
#include "utils/Conversions.h"

#include <ppltasks.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Platform::String;
using voicemynews::app::win10::components::MainMenu;
using voicemynews::app::win10::components::OnMenuExpanded;
using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::bindings::events::EventLoopBinding;
using voicemynews::app::win10::bindings::events::EventHandler;
using voicemynews::app::win10::js::JsApp;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using Windows::ApplicationModel::Core::CoreApplication;
using Windows::Data::Json::JsonArray;
using Windows::UI::Core::CoreDispatcherPriority;
using Windows::UI::Core::DispatchedHandler;

using voicemynews::core::events::kAppNavigationMenuLoad;
using voicemynews::core::events::kAppNavigationMenuLoaded;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
namespace components {
/**
 * \brief This class provides the unit tests for MainMenu component.
 */
TEST_CLASS(MainMenuTests) {
public:
    TEST_METHOD(MainMenuTestsOpenMenuWorksOk) {
        MainMenu^ mainMenu;
        auto dispatcher = CoreApplication::CreateNewView()->Dispatcher;

        concurrency::create_task(dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([&mainMenu] {
            mainMenu = ref new MainMenu();
            bool menuExpanded = mainMenu->IsMenuExpanded;

            mainMenu->MenuExpanded += ref new OnMenuExpanded([&menuExpanded](bool isMenuExpanded) {
                menuExpanded = isMenuExpanded;
            });


            Assert::IsFalse(mainMenu->IsMenuExpanded);
            Assert::AreEqual(mainMenu->IsMenuExpanded, menuExpanded);

            mainMenu->DoClickExpandButton();
            Assert::IsTrue(mainMenu->IsMenuExpanded);
            Assert::AreEqual(mainMenu->IsMenuExpanded, menuExpanded);

            mainMenu->DoClickExpandButton();
            Assert::IsFalse(mainMenu->IsMenuExpanded);
            Assert::AreEqual(mainMenu->IsMenuExpanded, menuExpanded);
        }))).wait();

        Assert::IsNotNull(mainMenu);
    }

    TEST_METHOD(MainMenuTestsLoadMenuItemsOk) {
        auto eventLoop = ref new EventLoopBinding();
        auto jsBackend = ref new JsApp(eventLoop);
        auto menuLoadTriggered = false;

        MainMenu^ mainMenu;
        auto dispatcher = CoreApplication::CreateNewView()->Dispatcher;

        eventLoop->On(ConvertStdStrToPlatform(kAppNavigationMenuLoad),
            ref new EventHandler([&menuLoadTriggered](EventDataBinding^ evtData) {
            menuLoadTriggered = true;
        }));

        concurrency::create_task(dispatcher->RunAsync(CoreDispatcherPriority::Normal,
                                    ref new DispatchedHandler([&mainMenu, &jsBackend] {
            mainMenu = ref new MainMenu(jsBackend);
        }))).wait();

        eventLoop->ProcessEvents();

        String^ menuContent = "[{\"icon\": \"/Assets/icons/menuitems/preferences.png\",\"label\" : \"Preferences\",\"type\" : \"item\", \"data\" : {\"evtName\": \"js:menuitems:openPreferences\"}},";
        menuContent += "{\"icon\": \"/Assets/icons/menuitems/genius.png\", \"label\" : \"Genius news\", \"type\" : \"item\", \"data\" : {\"evtName\": \"js:menuitems:openGenius\"}}]";
        eventLoop->Emit(ConvertStdStrToPlatform(kAppNavigationMenuLoaded), ref new EventDataBinding(menuContent));

        auto menuContentObj = JsonArray::Parse(menuContent);

        Assert::IsNotNull(mainMenu);
        Assert::IsTrue(menuLoadTriggered);

        eventLoop->ProcessEvents();

        concurrency::create_task(dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([&mainMenu, &menuContentObj]() {
            Assert::IsNotNull(mainMenu->MenuItems);
            auto index = 0;

            for (auto menuIt = mainMenu->MenuItems->First(); menuIt->HasCurrent; menuIt->MoveNext()) {
                auto expectedObj = menuContentObj->GetAt(index++)->ToString();
                auto currObj = menuIt->Current->ToString();
                Assert::AreEqual(expectedObj, currObj);
            }
        }))).wait();
    }
};

}
}
}
}
}