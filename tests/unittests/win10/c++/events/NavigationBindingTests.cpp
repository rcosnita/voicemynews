#include "pch.h"
#include "CppUnitTest.h"

#include "events/EventNames.h"
#include "utils/Conversions.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Platform::Object;
using Platform::String;
using Windows::UI::Xaml::Controls::Frame;
using Windows::UI::Xaml::Interop::TypeName;

using voicemynews::app::win10::bindings::events::IAppNavigator;
using voicemynews::app::win10::bindings::events::EventDataBinding;
using voicemynews::app::win10::bindings::events::NavigationBinding;
using voicemynews::app::win10::pages::UserPreferencesPage;
using voicemynews::app::win10::pages::GeniusNewsPage;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;

using voicemynews::core::events::kMenuItemOpenGeniusNews;
using voicemynews::core::events::kMenuItemOpenPreferences;

namespace voicemynews {
namespace tests {
namespace unittesting {
namespace events {
/**
 * \brief This class provides a mock which allows us to simulate navigation.
 */
ref class AppNavigatorMock : public IAppNavigator {
internal:
    TypeName pageType;
    EventDataBinding^ navigationData;

public:
    virtual bool Navigate(TypeName pageType, EventDataBinding^ navigationData) {
        this->pageType = pageType;
        this->navigationData = navigationData;

        return true;
    }
};

/**
 * \brief This class provides the navigation binding unit tests which ensures all methods work as expected.
 */
TEST_CLASS(NavigationBindingTests) {
public:
    TEST_METHOD_INITIALIZE(NavigationBindingTestsSetUp) {
        appNavigator_ = ref new AppNavigatorMock();
        navigationBinding_ = ref new NavigationBinding(appNavigator_);
    }

    TEST_METHOD(NavigationBindingTestsNavigateByEventGeniusNewsOk) {
        TestNavigateByEventOkTemplate(ConvertStdStrToPlatform(kMenuItemOpenGeniusNews),
            GeniusNewsPage::typeid, "GeniusNews", ref new EventDataBinding("{}"));
    }

    TEST_METHOD(NavigationBindingTestsNavigateByEventUserPreferencesOk) {
        TestNavigateByEventOkTemplate(ConvertStdStrToPlatform(kMenuItemOpenPreferences),
            UserPreferencesPage::typeid, "UserPreferences", ref new EventDataBinding("{}"));
    }

    TEST_METHOD(NavigationBindingTestsEventNameNotFound) {
        Assert::IsFalse(navigationBinding_->NavigateByEvent("evt:not:found:provided", nullptr));
    }

    TEST_METHOD(NavigationBindingTestsNullPtrNavigationDataOk) {
        TestNavigateByEventOkTemplate(ConvertStdStrToPlatform(kMenuItemOpenPreferences),
            UserPreferencesPage::typeid, "UserPreferences", nullptr);
    }

private:
    NavigationBinding^ navigationBinding_;
    AppNavigatorMock^ appNavigator_;

private:
    /**
     * \brief This method provides the template for testing NavigateByEvent success scenarios.
     */
    void TestNavigateByEventOkTemplate(String^ evtName, TypeName pageType, String^ pageName,
        EventDataBinding^ navigationData = nullptr) {
        Assert::IsTrue(navigationBinding_->NavigateByEvent(evtName, navigationData),
            (pageName + " page not reached.")->Data());
        Assert::AreEqual(pageType.Name, appNavigator_->pageType.Name,
            String::Concat(L"Other page reached instead of ", pageName));
        Assert::IsTrue(navigationData == appNavigator_->navigationData, L"Incorrect navigation data.");

    }
};
}
}
}
}