#include "pch.h"
#include "utils/Conversions.h"
#include "voice/VoiceSupport.h"

#include <memory>

using voicemynews::app::win10::bindings::news::VoiceBinding;
using voicemynews::core::voice::VoiceSupport;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;

using ReadingProgressHandler = voicemynews::app::win10::bindings::news::ReadingProgressHandler;
using ReadingPausedHandler = voicemynews::app::win10::bindings::news::ReadingPausedHandler;
using ReadingResumedHandler = voicemynews::app::win10::bindings::news::ReadingResumedHandler;
using ReadingDoneHandler = voicemynews::app::win10::bindings::news::ReadingDoneHandler;
using VoiceBinding = voicemynews::app::win10::bindings::news::VoiceBinding;

namespace voicemynews {
namespace core {
namespace voice {

/**
 * \brief Provides the windows implementation for voice support contract.
 *
 * The implementation relies on the VoiceBinding projection.
 */
class VoiceSupportWin : public voicemynews::core::voice::VoiceSupportAbstract {
    using VoiceReadingNotifications = voicemynews::core::voice::VoiceReadingNotifications;
    using VoiceReadingNotificationsBinding = voicemynews::app::win10::bindings::news::VoiceReadingNotifications;
public:
    VoiceSupportWin(VoiceBinding^ voiceBinding)
        : voiceBinding_(voiceBinding)
    {
    }

    virtual void ReadText(std::string text, std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
    {
        auto textStr = ConvertStdStrToPlatform(text);
        voiceBinding_->ReadText(textStr, ref new voicemynews::app::win10::bindings::news::VoiceReadingNotifications(
            ref new ReadingProgressHandler([&readingCallbacks](int64 currPos) {
            readingCallbacks->whenPlayheadChanged()(static_cast<long>(currPos));
        }), ref new ReadingPausedHandler([&readingCallbacks](int64 currPos) {
            readingCallbacks->whenResumed()(static_cast<long>(currPos));
        }), ref new ReadingResumedHandler([&readingCallbacks](int64 currPos) {
            readingCallbacks->whenResumed()(static_cast<long>(currPos));
        }), ref new ReadingDoneHandler([&readingCallbacks]() {
            readingCallbacks->whenDone()();
        })));
    }

    virtual void ReadSsml(std::string ssmlText, std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
    {
        throw ref new Platform::NotImplementedException("SSML support not implemented yet ...");
    }

    virtual void Pause(std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
    {
        voiceBinding_->Pause(ConvertToCallbacksBinding(readingCallbacks));
    }

    virtual void Resume(std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
    {
        voiceBinding_->Resume(ConvertToCallbacksBinding(readingCallbacks));
    }

private:
    /**
     * \brief Provides a conversion method between the native callbacks holder and the winrt ref class.
     */
    VoiceReadingNotificationsBinding^ ConvertToCallbacksBinding(std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
    {
        return ref new VoiceReadingNotificationsBinding(
            ref new ReadingProgressHandler([&readingCallbacks](int64 currPos) {
            readingCallbacks->whenPlayheadChanged()(static_cast<long>(currPos));
        }), ref new ReadingPausedHandler([&readingCallbacks](int64 currPos) {
            readingCallbacks->whenResumed()(static_cast<long>(currPos));
        }), ref new ReadingResumedHandler([&readingCallbacks](int64 currPos) {
            readingCallbacks->whenResumed()(static_cast<long>(currPos));
        }), ref new ReadingDoneHandler([&readingCallbacks]() {
            readingCallbacks->whenDone()();
        }));
    }

private:
    VoiceBinding^ voiceBinding_;
};

std::shared_ptr<VoiceSupportAbstract> GetVoiceSupportInstance()
{
    return std::make_shared<VoiceSupportWin>(VoiceSupport::GetInstance());
}
}
}
}