#include "pch.h"
#include "bindings/news/VoiceBinding.h"
#include "JsApp.h"

#include <cmath>
#include <ppltasks.h>

using Platform::String;
using Windows::Foundation::TypedEventHandler;
using Windows::Media::Core::MediaSource;
using Windows::Media::Playback::MediaPlayerAudioDeviceType;

using voicemynews::app::win10::bindings::events::JsLoopEnqueuedTask;

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
    player_->Source = MediaSource::CreateFromStream(speechStream, speechStream->ContentType);
    player_->Play();
}

void VoiceBinding::Pause(VoiceReadingNotifications^ notifications)
{
    player_->Pause();
    notifications->WhenPaused(GetCurrentLocation());
}

void VoiceBinding::Resume(VoiceReadingNotifications^ notifications)
{
    player_->Play();
    notifications->WhenResumed(GetCurrentLocation());
}

void VoiceBinding::Skip()
{
    player_->Pause();
    player_->Source = nullptr;
}

VoiceBinding::PlayheadLocation VoiceBinding::GetCurrentLocation()
{
    auto currPositionMs = player_->PlaybackSession->Position.Duration / long(std::pow(10, 4));
    return currPositionMs;
}

}
}
}
}
}

namespace voicemynews {
namespace core {
namespace voice {

VoiceSupport::VoiceBinding^ VoiceSupport::GetInstance()
{
    return ref new VoiceSupport::VoiceBinding();
}

voicemynews::app::win10::bindings::news::VoiceReadingNotifications^ VoiceSupport::GetNotificationsInstance(
        VoiceSupport::ReadingProgressHandler^ progressCallback,
        VoiceSupport::ReadingPausedHandler^ pausedCallback,
        VoiceSupport::ReadingResumedHandler^ resumedCallback,
        VoiceSupport::ReadingDoneHandler^ doneCallback)
{
    return ref new voicemynews::app::win10::bindings::news::VoiceReadingNotifications(progressCallback,
        pausedCallback, resumedCallback, doneCallback);
}
}
}
}