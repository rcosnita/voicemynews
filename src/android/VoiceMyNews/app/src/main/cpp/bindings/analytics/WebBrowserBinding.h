#ifndef VOICEMYNEWSANDROID_ANALYTICS_WEBBROWSERBINDING_H_
#define VOICEMYNEWSANDROID_ANALYTICS_WEBBROWSERBINDING_H_

#include "analytics/WebBrowser.h"
#include <jni.h>

namespace voicemynews {
namespace app {
namespace android {
namespace analytics {
/**
 * \brief Provides a web browser native wrapper over android web browser.
 *
 * All methods will delegate to the java implementation of the web browser.
 */
class WebBrowserBinding : public voicemynews::core::analytics::WebBrowser
{
private:
    WebBrowserBinding() = default;
    friend class voicemynews::core::analytics::WebBrowser;

public:
    void LoadContent(std::string webPageContent) override;

    void SendEvent(std::string evtName, std::string evtData) override;
};
}
}
}
}

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Allows native side to be notified about a java web browser wrapper object.
 *
 * We use it in order to provide a reliable implementation for web browser contract required by analytics
 * layer.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_analytics_WebBrowserWrapper_initNativeBrowser(
    JNIEnv *env,
    jobject objInst);

#ifdef __cplusplus
};
#endif

#endif /* VOICEMYNEWSANDROID_ANALYTICS_WEBBROWSERBINDING_H_ */