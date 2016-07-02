#include "pch.h"

#include <mutex>
#include <condition_variable>

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace app {
using Windows::ApplicationModel::Core::CoreApplication;
using Windows::UI::Core::CoreDispatcherPriority;
using Windows::UI::Xaml::Controls::Button;
using Windows::UI::Xaml::RoutedEventArgs;

using voicemynews::app::win10::MainPage;

TEST_CLASS(MainPageTests)
{
public:
    TEST_METHOD(MainPageInstantiateOk)
    {
        std::mutex m;
        std::condition_variable cv;

        MainPage^ page = nullptr;

        auto dispatcher = CoreApplication::CreateNewView()->Dispatcher;
        auto results = dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([&m, &cv, &page] {
            std::lock_guard<std::mutex> lk(m);
            page = ref new MainPage();

            cv.notify_one();
        }));

        {
            std::unique_lock<std::mutex> lk(m);
            cv.wait(lk, [&page] {return page != nullptr; });
        }

        Assert::IsTrue(true);
    }
};
}
}
}
}