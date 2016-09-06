#ifndef VoiceMyNewsAppUI_bindings_news_VoiceBinding
#define VoiceMyNewsAppUI_bindings_news_VoiceBinding

#include "bindings/events/EventLoopBinding.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace news {
/**
 * This delegate can be used when triggering read in order to receive notifications about when the reading is done.
 */
public delegate void ReadingDone();

/**
 * Provides a unified model of notifications which can be received by consumers of voice logic.
 */
public ref class VoiceNotifications sealed
{
public:
    property ReadingDone^ WhenDone {
        ReadingDone^ get();
    }
public:
    VoiceNotifications(ReadingDone^ whenDone);

private:
    ReadingDone^ whenDone_;
};

/**
 * \brief This class provides the voice reading support which can be used in js business logic.
 */
public ref class VoiceBinding sealed
{
using MediaPlayer = Windows::Media::Playback::MediaPlayer;
using SpeechSynthesizer = Windows::Media::SpeechSynthesis::SpeechSynthesizer;
using SpeechStream = Windows::Media::SpeechSynthesis::SpeechSynthesisStream;
public:
    VoiceBinding();
    virtual ~VoiceBinding();

    /**
     * \brief This method provides the logic for transforming the given text into speech.
     *
     * This is the most basic api and consumers will usually do some processing / aggregation between invoking
     * this method.
     */
    void ReadText(Platform::String^ paragraph, VoiceNotifications^ notifications);

private:
    /**
     * \brief Plays the given stream on the hidden media element binded to the instance.
     */
    void PlayStream(SpeechStream^ speechStream);

private:
    MediaPlayer^ player_;
    SpeechSynthesizer^ speechSynthesizer_;
    voicemynews::app::win10::bindings::events::EventLoopBinding^ jsEventLoop_;

    Windows::Foundation::EventRegistrationToken mediaEndedToken_;
};

}
}
}
}
}

#endif /* VoiceMyNewsAppUI_bindings_news_VoiceBinding */
