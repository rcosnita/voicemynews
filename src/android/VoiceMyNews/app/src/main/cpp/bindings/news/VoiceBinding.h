#ifndef VoiceMyNewsAndroid_bindings_news_VoiceBinding_H_
#define VoiceMyNewsAndroid_bindings_news_VoiceBinding_H_

#include "voice/VoiceSupport.h"
#include "v8.h"
#include <iostream>
#include <jni.h>

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

/**
 * \brief Initializes all jni related objects which are required by voice binding implementation.
 *
 * It simply saves pointers to VoiceSupportAndroid / VoiceSupportActions methods so that it can invoke
 * them at a later stage.
 */
void InitVoiceSupportBinding(JNIEnv* env, jobject voiceSupportInstance);
}
}
}
}
}

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Provides the implementation of com.voicemynews.core.bindings.news.VoiceSupportActions.invokeWhenPlayheadChangedFn.
 *
 * Internally it invokes the specified fnPtr and passes to it the position received as argument.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_news_VoiceSupportActions_invokeWhenPlayheadChangedFn(
    JNIEnv* env,
    jobject objInst,
    jint pos,
    jlong fnPtr);

/**
 * \brief Provides the implementation of com.voicemynews.core.bindings.news.VoiceSupportActions.invokeWhenPausedFn.
 *
 * Internally it invokes the specified fnPtr and passes to it the position received as argument.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_news_VoiceSupportActions_invokeWhenPausedFn(
    JNIEnv* env,
    jobject objInst,
    jlong pos,
    jlong fnPtr);

/**
 * \brief Provides the implementation of com.voicemynews.core.bindings.news.VoiceSupportActions.invokeWhenResumedFn.
 *
 * Internally it invokes the specified fnPtr and passes to it the position received as argument.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_news_VoiceSupportActions_invokeWhenResumedFn(
    JNIEnv* env,
    jobject objInst,
    jlong pos,
    jlong fnPtr);

/**
 * \brief Provides the implementation of com.voicemynews.core.bindings.news.VoiceSupportActions.invokeWhenDoneFn.
 *
 * Internally it invokes the specified fnPtr.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_news_VoiceSupportActions_invokeWhenDoneFn(
    JNIEnv* env,
    jobject objInst,
    jlong fnPtr);

#ifdef __cplusplus
};
#endif

#endif /* VoiceMyNewsAndroid_bindings_news_VoiceBinding_H_ */