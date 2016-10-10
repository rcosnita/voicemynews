#ifndef VoiceMyNewsCore_voice_VoiceSupport_H_
#define VoiceMyNewsCore_voice_VoiceSupport_H_

#include <functional>
#include <memory>
#include <string>

namespace voicemynews {
namespace core {
namespace voice {
/**
 * \brief Provides a contract which can be used by users in order to receive notifications about voice playback.
 *
 * Consumers of the voice api can provide specific callbacks which are going to be invoked during the reading process.
 */
class VoiceReadingNotifications {
public:
    VoiceReadingNotifications(std::function<void(int)>&& whenPlayheadChanged,
        std::function<void(long)>&& whenPaused,
        std::function<void(long)>&& whenResumed,
        std::function<void()>&& whenDone);

    /**
     * \brief Returns the callback method registered for being invoked when playback ends.
     */
    const std::function<void()>& whenDone() const;

    /**
     * \brief Returns the callback registered for being invoked whenever the playhead changed.
     *
     * The callback receives the new playhead location.
     */
    const std::function<void(long)>& whenPlayheadChanged() const;

    /**
     * \brief Returns the callback registered for being invoked whenever the playback is paused.
     *
     * The callback receives the current playhead location.
     */
    const std::function<void(long)>& whenPaused() const;

    /**
     * \brief Returns the callback registered for being invoked whenever the playback is resumed.
     *
     * The callback receives the current playhead location.
     */
    const std::function<void(long)>& whenResumed() const;

private:
    std::function<void()> whenDone_;
    std::function<void(long)> whenPlayheadChanged_;
    std::function<void(long)> whenPaused_;
    std::function<void(long)> whenResumed_;
};

/**
 * \brief This class provides the contract for voice which must be provided by each platform.
 *
 * In the future, we might provide a cross platform implementation, but at this moment this is not feasible or
 * cost effective.
 */
class VoiceSupportAbstract {
public:
    /**
     * \brief Read text and gives user notifications about the reading action. 
     * \param text the plain text which must be synthesized using tts.
     * \param readingCallbacks the reading callbacks used for being notified about playback progress.
     */
    virtual void ReadText(std::string text, std::shared_ptr<VoiceReadingNotifications> readingCallbacks) = 0;

    /**
     * \brief Read the given ssml text and notifies consumer about the progress.
     *
     * \param ssmlText the text formatted using SSML markup language wich must be synthesized using tts.
     * \param readingCallbacks the reading callbacks used for being notified about playback progress.
     */
    virtual void ReadSsml(std::string ssmlText, std::shared_ptr<VoiceReadingNotifications> readingCallbacks) = 0;

    /**
     * \brief Pause the current stream and notifies consumer about the progress.
     *
     * \param readingCallbacks the reading callbacks used for being notified about playback progress.
     */
    virtual void Pause(std::shared_ptr<VoiceReadingNotifications> readingCallbacks) = 0;

    /**
     * \brief Resume the current stream and notifies consumer about the progress.
     *
     * \param readingCallbacks the reading callbacks used for being notified about playback progress.
     */
    virtual void Resume(std::shared_ptr<VoiceReadingNotifications> readingCallbacks) = 0;

    /**
     * \brief Skips the current stream into a synchronous maner.
     *
     * Once invoked and finished, developers can safely assume that they can play a new stream.
     */
    virtual void Skip() = 0;
};

/**
 * \brief This factory method obtains an instance of voice support implementation which can be used in code.
 *
 * Each platform will provide a specific implementation for this method. In addition to this method each class
 * must provide an implementation VoiceSupport factory which contains GetInstance and GetNotificationsInstance methods.
 * These will be binded / projected into js engine chose for the platform under voicemynews.core.voice.VoiceSupport
 * context.
 */
std::shared_ptr<VoiceSupportAbstract> GetVoiceSupportInstance();
}
}
}

#endif