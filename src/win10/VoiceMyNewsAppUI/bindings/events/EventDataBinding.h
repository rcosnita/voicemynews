#ifndef VoiceMyNewsAppUI_bindings_events_EventDataBinding_H_
#define VoiceMyNewsAppUI_bindings_events_EventDataBinding_H_

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
using Platform::String;

/**
* \class EventLoopData
* \brief This class provides a generic event data object which can be used from javascript.
*/
public ref class EventDataBinding sealed {
public:
    EventDataBinding(String^ data);

    property String^ EvtData {
        String^ get();
    };

private:
    String^ data_;
};
}
}
}
}
}
#endif /* VoiceMyNewsAppUI_bindings_events_EventDataBinding_H_ */