#ifndef VoiceMyNewsAppUI_bindings_news_VoiceBinding
#define VoiceMyNewsAppUI_bindings_news_VoiceBinding

#include "bindings/events/EventLoopBinding.h"
#include "voice/VoiceSupport.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace news {
/**
 * This delegate can be used when triggering read in order to receive notifications when the reading is done.
 */
public delegate void ReadingDoneHandler();

/**
 * This delegate can be used when triggering read in order to receive notifications when the reading is paused.
 */
public delegate void ReadingPausedHandler(int64 currPos);

/**
 * This delegate can be used when triggering read in order to receive notifications when the reading is resumed.
 */
public delegate void ReadingResumedHandler(int64 currPos);

/**
 * This delegate can be used when triggering read in order to receive notifications when the reading is in progress.
 */
public delegate void ReadingProgressHandler(int64 currPos);

/**
 * \brief Provides a unified model of notifications which can be received by consumers of voice logic.
 */
public ref class VoiceReadingNotifications sealed
{
public:
    property ReadingDoneHandler^ WhenDone {
        ReadingDoneHandler^ get();
    }

    property ReadingProgressHandler^ WhenProgress {
        ReadingProgressHandler^ get();
    }

    property ReadingPausedHandler^ WhenPaused {
        ReadingPausedHandler^ get();
    }

    property ReadingResumedHandler^ WhenResumed {
        ReadingResumedHandler^ get();
    }

public:
    VoiceReadingNotifications(ReadingProgressHandler^ whenProgress,
        ReadingPausedHandler^ whenPaused,
        ReadingResumedHandler^ whenResumed,
        ReadingDoneHandler^ whenDone);

private:
    ReadingProgressHandler^ whenProgress_;
    ReadingPausedHandler^ whenPaused_;
    ReadingResumedHandler^ whenResumed_;
    ReadingDoneHandler^ whenDone_;
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
    void ReadText(Platform::String^ paragraph, VoiceReadingNotifications^ notifications);

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

/**
 * \brief Provides the windows implementation for voice support contract.
 *
 * The implementation relies on the VoiceBinding projection.
 */
class VoiceSupportWin : public voicemynews::core::voice::VoiceSupportAbstract {
using VoiceReadingNotifications = voicemynews::core::voice::VoiceReadingNotifications;
public:
    VoiceSupportWin(VoiceBinding^ voiceBinding);

    virtual void ReadText(std::string text, std::shared_ptr<VoiceReadingNotifications> readingCallbacks);

    virtual void ReadSsml(std::string ssmlText, std::shared_ptr<VoiceReadingNotifications> readingCallbacks);

private:
    VoiceBinding^ voiceBinding_;
};

}
}
}
}
}

#endif /* VoiceMyNewsAppUI_bindings_news_VoiceBinding */
