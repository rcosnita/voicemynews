#ifndef VoiceMyNewsAndroid_bindings_network_HttpClientBinding_H_
#define VoiceMyNewsAndroid_bindings_network_HttpClientBinding_H_

#include "network/HttpClientInterface.h"
#include "v8.h"

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Provides the native implementation for initializing the http client c++ layer.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_network_HttpClientBinding_initializeNative(
    JNIEnv* env,
    jclass objClass,
    jobject httpClientObj);

/**
 * \brief Provides the native implementation which can invoke the js callback binded to the action.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_network_HttpClientBindingGetAction_invokeJsCallback(
    JNIEnv* env,
    jobject thisObj,
    jobject responseData);

/**
 * \brief Provides the native implementation which can invoke the js callback binded to the action.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_network_HttpClientBindingParseStringContentAction_invokeJsCallback(
    JNIEnv* env,
    jobject thisObj,
    jstring response);

#ifdef __cplusplus
};
#endif

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
/**
 * \brief Provides the implementation for http client which can be binded to javascript.
 */
class HttpClientBinding : public voicemynews::core::network::HttpClientInterface {
public:
    HttpClientBinding() = default;
    virtual ~HttpClientBinding() = default;

    /**
     * \brief Provides an implementation for http get request which can be safely used in native code.
     */
    virtual void Get(const std::string& url, const std::map<std::string, std::string>& headers,
                     const std::map<std::string, std::string>& queryParams,
                     voicemynews::core::network::HttpClientResponseStringCallback handleResponse) override;

public:
    /**
     * \brief Implements the code which binds the android http client into javascript engine.
     */
    static void WireToJs(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> obj);
};
}
}
}
}

#endif /* VoiceMyNewsAndroid_bindings_network_HttpClientBinding_H_ */