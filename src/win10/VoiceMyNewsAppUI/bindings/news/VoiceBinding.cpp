#include "pch.h"
#include "bindings/news/VoiceBinding.h"
#include "utils/Conversions.h"
#include "JsApp.h"

#include <ppltasks.h>

using Platform::String;
using Windows::Foundation::TypedEventHandler;
using Windows::Media::Playback::MediaPlayerAudioDeviceType;

using voicemynews::app::win10::bindings::events::JsLoopEnqueuedTask;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace news {
VoiceReadingNotifications::VoiceReadingNotifications(ReadingProgressHandler^ whenProgress,
    ReadingPausedHandler^ whenPaused,
    ReadingResumedHandler^ whenResumed,
    ReadingDoneHandler^ whenDone)
    : whenProgress_(whenProgress),
        whenPaused_(whenPaused),
        whenResumed_(whenResumed),
        whenDone_(whenDone)
{
}

ReadingProgressHandler^ VoiceReadingNotifications::WhenProgress::get()
{
    return whenProgress_;
}

ReadingPausedHandler^ VoiceReadingNotifications::WhenPaused::get()
{
    return whenPaused_;
}

ReadingResumedHandler^ VoiceReadingNotifications::WhenResumed::get()
{
    return whenResumed_;
}

ReadingDoneHandler^ VoiceReadingNotifications::WhenDone::get()
{
    return whenDone_;
}

VoiceBinding::VoiceBinding()
{
    jsEventLoop_ = voicemynews::app::win10::js::JsApp::GetInstance()->GetEventLoop();
    speechSynthesizer_ = ref new Windows::Media::SpeechSynthesis::SpeechSynthesizer();

    player_ = ref new VoiceBinding::MediaPlayer();
    player_->AutoPlay = false;
    player_->IsMuted = false;
    player_->AudioDeviceType = MediaPlayerAudioDeviceType::Multimedia;
}

VoiceBinding::~VoiceBinding() { }

void VoiceBinding::ReadText(Platform::String^ paragraph, VoiceReadingNotifications^ notifications)
{
    mediaEndedToken_ = player_->MediaEnded += ref new TypedEventHandler<MediaPlayer^, Platform::Object^>([this, notifications](MediaPlayer^ player, Platform::Object^ data) {
        jsEventLoop_->EnqueueTask(ref new JsLoopEnqueuedTask([this, notifications]() {
            if (notifications != nullptr) {
                notifications->WhenDone();
            }

            player_->MediaEnded -= mediaEndedToken_;
        }));
    });

    concurrency::create_task(speechSynthesizer_->SynthesizeTextToStreamAsync(paragraph))
        .then([this](concurrency::task<VoiceBinding::SpeechStream^> streamTask) {
        auto stream = streamTask.get();

        PlayStream(stream);
    });
}

void VoiceBinding::PlayStream(VoiceBinding::SpeechStream^ speechStream)
{
    player_->SetStreamSource(speechStream);
    player_->Play();
}

VoiceSupportWin::VoiceSupportWin(VoiceBinding^ voiceBinding) 
    : voiceBinding_(voiceBinding)
{
}

void VoiceSupportWin::ReadText(std::string text, std::shared_ptr<VoiceSupportWin::VoiceReadingNotifications> readingCallbacks)
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

void VoiceSupportWin::ReadSsml(std::string ssmlText, std::shared_ptr<VoiceSupportWin::VoiceReadingNotifications> readingCallbacks)
{
    throw ref new Platform::NotImplementedException("SSML support not implemented yet ...");
}
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace voice {
using voicemynews::app::win10::bindings::news::ReadingProgressHandler;
using voicemynews::app::win10::bindings::news::ReadingPausedHandler;
using voicemynews::app::win10::bindings::news::ReadingResumedHandler;
using voicemynews::app::win10::bindings::news::ReadingDoneHandler;
using voicemynews::app::win10::bindings::news::VoiceBinding;
using voicemynews::app::win10::bindings::news::VoiceSupportWin;

/**
 * \brief Provides a factory implementation for obtaining voice providers and notification objects.
 */
public ref class VoiceSupport sealed
{
public:
    /**
     * \brief Obtains an instance of voice class which provides TTS algorithm.
     */
    static VoiceBinding^ GetInstance() { return ref new VoiceBinding(); }

    /**
     * \brief Obtains an instance of notifications class which allows js logic to receive playback events.
     */
    static voicemynews::app::win10::bindings::news::VoiceReadingNotifications^ GetNotificationsInstance(
        ReadingProgressHandler^ progressCallback,
        ReadingPausedHandler^ pausedCallback,
        ReadingResumedHandler^ resumedCallback,
        ReadingDoneHandler^ doneCallback)
    {
        return ref new voicemynews::app::win10::bindings::news::VoiceReadingNotifications(progressCallback,
            pausedCallback, resumedCallback, doneCallback);
    }
};

std::shared_ptr<VoiceSupportAbstract> GetVoiceSupportInstance()
{
    return std::make_shared<VoiceSupportWin>(VoiceSupport::GetInstance());
}
}
}
}