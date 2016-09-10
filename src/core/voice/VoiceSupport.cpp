#include "voice/VoiceSupport.h"

namespace voicemynews {
namespace core {
namespace voice {
VoiceReadingNotifications::VoiceReadingNotifications(std::function<void(int)>&& whenPlayheadChanged,
    std::function<void(long)>&& whenPaused,
    std::function<void(long)>&& whenResumed,
    std::function<void()>&& whenDone)
    : whenPlayheadChanged_(whenPlayheadChanged),
        whenPaused_(whenPaused),
        whenResumed_(whenResumed),
        whenDone_(whenDone)
{
}

const std::function<void()>& VoiceReadingNotifications::whenDone() const
{
    return whenDone_;
}

const std::function<void(long)>& VoiceReadingNotifications::whenPlayheadChanged() const
{
    return whenPlayheadChanged_;
}

const std::function<void(long)>& VoiceReadingNotifications::whenPaused() const
{
    return whenPaused_;
}

const std::function<void(long)>& VoiceReadingNotifications::whenResumed() const
{
    return whenResumed_;
}
}
}
}