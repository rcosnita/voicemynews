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

    /**
     * \brief Provides the logic for pausing the current running stream.
     *
     * This is a basic interruption based operation which can be invoked by consumers.
     */
    void Pause(VoiceReadingNotifications^ notifications);

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

namespace voicemynews {
namespace core {
namespace voice {
/**
* \brief Provides a factory implementation for obtaining voice providers and notification objects.
*/
public ref class VoiceSupport sealed
{
using ReadingProgressHandler = voicemynews::app::win10::bindings::news::ReadingProgressHandler;
using ReadingPausedHandler = voicemynews::app::win10::bindings::news::ReadingPausedHandler;
using ReadingResumedHandler = voicemynews::app::win10::bindings::news::ReadingResumedHandler;
using ReadingDoneHandler = voicemynews::app::win10::bindings::news::ReadingDoneHandler;
using VoiceBinding = voicemynews::app::win10::bindings::news::VoiceBinding;

public:
    /**
    * \brief Obtains an instance of voice class which provides TTS algorithm.
    */
    static VoiceBinding^ GetInstance();

    /**
    * \brief Obtains an instance of notifications class which allows js logic to receive playback events.
    */
    static voicemynews::app::win10::bindings::news::VoiceReadingNotifications^ GetNotificationsInstance(
        VoiceSupport::ReadingProgressHandler^ progressCallback,
        VoiceSupport::ReadingPausedHandler^ pausedCallback,
        VoiceSupport::ReadingResumedHandler^ resumedCallback,
        VoiceSupport::ReadingDoneHandler^ doneCallback);
};
}
}
}

#endif /* VoiceMyNewsAppUI_bindings_news_VoiceBinding */
