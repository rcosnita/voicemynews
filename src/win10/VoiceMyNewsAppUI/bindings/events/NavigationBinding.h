#ifndef VoiceMyNewsAppUI_bindings_events_NavigationBinding_H_
#define VoiceMyNewsAppUI_bindings_events_NavigationBinding_H_

#include "bindings/events/EventDataBinding.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
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

    /**
     * \brief this class provides access to a navigation instance.
     */
    static NavigationBinding^ GetInstance();

    /**
     * \brief This helper method allows consumers to navigate based on a givent event name.
     *
     * Each navigation by event can also receive specific event data which can influence the navigation.
     */
    void NavigateByEvent(String^ evtName, EventDataBinding^ evtData);

internal:
    /**
    * \brief This property provides access to the menu content view which can be used to change its content.
    */
    property Frame^ MenuContentView {
        Frame^ get();
        void set(Frame^ menuContentView);
    }

private:
    Frame^ menuContentView_;
    Windows::Foundation::Collections::IMap<String^, TypeName>^ menuNavigationMapping_;
};
}
}
}
}
}

#endif /* VoiceMyNewsAppUI_bindings_events_NavigationBinding_H_ */
