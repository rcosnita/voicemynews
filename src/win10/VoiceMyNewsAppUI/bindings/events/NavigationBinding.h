#ifndef VoiceMyNewsAppUI_bindings_events_NavigationBinding_H_
#define VoiceMyNewsAppUI_bindings_events_NavigationBinding_H_

#include "bindings/events/EventDataBinding.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
/**
 * \brief This interface defines the contract for navigating between various sections of the app.
 */
[Windows::Foundation::Metadata::WebHostHidden]
public interface class IAppNavigator {
    using TypeName = Windows::UI::Xaml::Interop::TypeName;

    /**
     * \brief Navigate to the given page type. During navigation, the navigationData will be passed to the page.
     *
     * \param pageType The xaml page where we want to navigate.
     * \param navigationData The navigation data we want to pass to the destination.
     * \returns true if the navigation has been successful and false otherwise.
     */
    bool Navigate(TypeName pageType, EventDataBinding^ navigationData);
};

/**
 * Provides an app navigator implementation which relies on a frame to do the actual UI xaml navigation.
 */
[Windows::Foundation::Metadata::WebHostHidden]
public ref class FrameAppNavigator sealed : public IAppNavigator {
using Frame = Windows::UI::Xaml::Controls::Frame;
using TypeName = Windows::UI::Xaml::Interop::TypeName;

public:
    FrameAppNavigator(Frame^ frame);

    /**
     * Navigate to the specified page type by using xaml standard frame to frame navigation.
     */
    virtual bool Navigate(TypeName pageType, EventDataBinding^ navigationData);

private:
    Frame^ frame_;
};

/**
 * \brief This class provides the layer which can navigate to various sections of the app.
 *
 * The navigation component is visible in js layer and can easily be used when necessary.
 */
public ref class NavigationBinding sealed {
using Frame = Windows::UI::Xaml::Controls::Frame;
using String = Platform::String;
using TypeName = Windows::UI::Xaml::Interop::TypeName;

public:
    NavigationBinding();

#pragma warning (disable: 4453)
    NavigationBinding(IAppNavigator^ appNavigator);

    /**
     * \brief this class provides access to a navigation instance.
     */
    static NavigationBinding^ GetInstance();

    /**
     * \brief This helper method allows consumers to navigate based on a givent event name.
     *
     * Each navigation by event can also receive specific event data which can influence the navigation.
     *
     * \param evtName the event name which uniquely identifies an application section.
     * \param evtData an object storing additional information which must be used by navigation context.
     */
    bool NavigateByEvent(String^ evtName, EventDataBinding^ evtData);

internal:
    /**
    * \brief This property provides access to the menu content navigator which can be used to change menu content.
    */
    property IAppNavigator^ MenuContentNavigator {
        IAppNavigator^ get();
        void set(IAppNavigator^ menuContentNavigator);
    }

private:
    IAppNavigator^ menuContentNavigator_;
    Windows::Foundation::Collections::IMap<String^, TypeName>^ menuNavigationMapping_;

private:
    /**
     * \brief This method initializes all properties / members.
     */
    void InitComponent();
};
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace events {
/**
 * \brief This class provides a factory for obtaining the navigation manager for the current platform.
 *
 * JS code can safely rely on this in order to implement navigation based on events.
 */
public ref class NavigationManagerPlatform sealed {
using NavigationBinding = voicemynews::app::win10::bindings::events::NavigationBinding;
public:
    /**
     * This method obtains the current navigation manager which can be used by the application.
     */
    static NavigationBinding^ GetInstance();
};
}
}
}

#endif /* VoiceMyNewsAppUI_bindings_events_NavigationBinding_H_ */
