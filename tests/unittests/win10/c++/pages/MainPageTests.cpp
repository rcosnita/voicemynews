#include "pch.h"
#include "CppUnitTest.h"

#include <ppltasks.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using Windows::ApplicationModel::Core::CoreApplication;
using Windows::UI::Core::CoreDispatcherPriority;
using Windows::UI::Core::DispatchedHandler;
using voicemynews::app::win10::MainPage;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
TEST_CLASS(MainPageTests)
{
public:
    TEST_METHOD(MainPageTestsMainPageInstantiatedOk) {
        MainPage^ page = nullptr;
        auto dispatcher = CoreApplication::CreateNewView()->Dispatcher;

        concurrency::create_task(dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([&page] {
            page = ref new MainPage();
        }))).wait();

        Assert::IsNotNull(page);
    }
};
}
}
}
}