#ifndef VoiceMyNewsAndroid_JsApp_H_
#define VoiceMyNewsAndroid_JsApp_H_

#include "bindings/events/EventLoopPlatform.h"
#include "io/fs/FileUtilsPlatform.h"

#include "libplatform/libplatform.h"
#include "v8.h"

#include <android/asset_manager.h>
#include <iostream>

namespace voicemynews {
namespace app {
namespace android {
namespace js {
/**
 * \brief Provides the javascript application bootstrap logic.
 *
 * It starts the V8 engine, binds all required layers and starts voicemynews business logic.
 */
class JsApp
{
public:
    using EventLoopPlatform = voicemynews::core::events::EventLoopPlatform;
    using FileUtils = voicemynews::core::io::fs::FileUtilsPlatform;

public:
    JsApp(EventLoopPlatform* eventLoop);
    virtual ~JsApp();

    /**
     * \brief Start the js business logic of voicemynews application.
     */
    void Start();

    /**
     * \brief Obtains the current event loop used by the js business logic.
     */
    EventLoopPlatform* GetEventLoop();

public:
    /**
     * \brief Obtains an instance of js app.
     *
     * We can have only one js app in the app at any given moment, so expect this to be a singleton.
     */
    static JsApp* GetInstance(JsApp::EventLoopPlatform* eventLoop = nullptr);

    /**
     * \brief Shutdowns the current js platform and cleanup all resources.
     *
     * Internally, it destroys the V8 platform. This is the last method which must be invoked before
     * the app exits.
     */
    static void ShutdownPlatform();

    /**
     * \brief Provides the one time logic which allows us to boot the js platform.
     *
     * Internally, it initializes the V8 platform. This method must be invoked first time the app
     * starts. If any other method from JsApp is called before this method the app will crash.
     */
    static void StartPlatform();

private:
    EventLoopPlatform* eventLoop_ = nullptr;
    FileUtils fileUtils_;
    v8::Isolate* isolate_ = nullptr;
    v8::Persistent<v8::Context> *persistentContext_ = nullptr;
    v8::Context::Scope *contextScope_ = nullptr;
    v8::Persistent<v8::ObjectTemplate> *voicemynewsObj = nullptr;
    v8::Persistent<v8::ObjectTemplate> *voicemynewsCoreObj = nullptr;

private:
    /**
     * \brief Binds the namespace which can provide the native requirejs support methods.
     */
    void BindRequireJsNativeSupport();

    /**
     * \brief Binds the require js support methods to v8 engine.
     */
    void BindRequireJsSupport();

    /**
     * \brief Binds the http client support methods to v8 engine.
     */
    void BindHttpClientSupport();

    /**
     * \brief Binds the navigation manager support methods to v8 engine.
     */
    void BindNavigationManagerSupport();

    /**
     * \brief Binds the voice support methods to v8 engine.
     */
    void BindVoiceSupport();

    /**
     * \brief Loads the app.js script and bootstraps the business logic.
     */
    void StartApp();

    /**
     * \brief Provides a standard wait to handle exceptions and log them if necessary.
     *
     * The try catch block enabled for the current js code.
     */
    template<typename T>
    void HandleJsException(v8::TryCatch& tryCatch, v8::MaybeLocal<T>& result)
    {
        if (!result.IsEmpty()) {
            return;
        }

        v8::Local<v8::Value> exception = tryCatch.Exception();
        v8::String::Utf8Value exceptionStr(exception);
        std::string exceptionMessage = *exceptionStr;

        std::cout << exceptionMessage << std::endl;
        throw std::exception();
    }
};
}
}
}
}

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Provides the logic for starting the js application platform.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_voicemynews_JsApp_startPlatform(
    JNIEnv* env,
    jclass objClass,
    jlong nativeEmitterPtr,
    jobject assetManager);

/**
 * \brief Provides the logic for stopping the js application platform.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_voicemynews_JsApp_shutdownPlatform(
    JNIEnv* env,
    jclass objClass,
    jlong nativeEmitterPtr);

#ifdef __cplusplus
}
#endif

#endif /* VoiceMyNewsAndroid_JsApp_H_ */
