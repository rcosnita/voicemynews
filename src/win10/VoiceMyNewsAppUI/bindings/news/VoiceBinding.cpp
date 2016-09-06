#include "pch.h"
#include "bindings/news/VoiceBinding.h"
#include "JsApp.h"

#include <ppltasks.h>

using Platform::String;
using Windows::Foundation::TypedEventHandler;
using Windows::Media::Playback::MediaPlayerAudioDeviceType;

using voicemynews::app::win10::bindings::events::JsLoopEnqueuedTask;

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace news {
VoiceNotifications::VoiceNotifications(ReadingDone^ whenDone)
    : whenDone_(whenDone)
{
}

ReadingDone^ VoiceNotifications::WhenDone::get()
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

void VoiceBinding::ReadText(Platform::String^ paragraph, VoiceNotifications^ notifications)
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
}
}
}
}
}