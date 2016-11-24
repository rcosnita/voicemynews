#ifndef VoiceMyNewsAndroid_bindings_news_VoiceBinding_H_
#define VoiceMyNewsAndroid_bindings_news_VoiceBinding_H_

#include "voice/VoiceSupport.h"
#include "v8.h"

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace news {
/**
 * \brief Voice support implementation for android platform.
 *
 * It relies on native APIs in order to synthesize and playback streams to audio backend.
 */
class VoiceBinding : public voicemynews::core::voice::VoiceSupportAbstract
{
public:
    using VoiceReadingNotifications = voicemynews::core::voice::VoiceReadingNotifications;

    /**
     * \brief Binds the voice support logic to js v8 engine.
     */
    static void WireToJs(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> obj);

public:
    VoiceBinding() = default;
    virtual ~VoiceBinding() = default;

    virtual void ReadText(std::string text, std::shared_ptr<VoiceReadingNotifications> readingCallbacks) override;

    virtual void ReadSsml(std::string ssmlText, std::shared_ptr<VoiceReadingNotifications> readingCallbacks) override;

    virtual void Pause(std::shared_ptr<VoiceReadingNotifications> readingCallbacks) override;

    virtual void Resume(std::shared_ptr<VoiceReadingNotifications> readingCallbacks) override;

    virtual void Skip() override;
};
}
}
}
}
}

#endif /* VoiceMyNewsAndroid_bindings_news_VoiceBinding_H_ */