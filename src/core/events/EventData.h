#ifndef VoiceMyNewsCore_events_EventData_H_
#define VoiceMyNewsCore_events_EventData_H_

namespace voicemynews {
namespace core {
namespace events {
/**
 * This class provides a generic event data object used to describe event data content.
 */
template<typename T>
class EventData {
public:
    EventData(T data) : data_(data) { }

    T data() const { return data_; }
private:
    T data_;
};
}
}
}

#endif // VoiceMyNewsCore_events_EventData_H_