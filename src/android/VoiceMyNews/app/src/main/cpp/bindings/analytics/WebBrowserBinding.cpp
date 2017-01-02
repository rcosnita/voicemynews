#include "WebBrowserBinding.h"

#include <memory>
#include <exception>

using voicemynews::app::android::bindings::analytics::WebBrowserBinding;
using voicemynews::core::analytics::WebBrowser;

static JavaVM* CurrJavaVM = nullptr;
static jobject WebBrowserObjInst = nullptr;
static jmethodID WebBrowserLoadContent = nullptr;
static jmethodID WebBrowserSendEvent = nullptr;
static std::shared_ptr<WebBrowser> WebBrowserNative = nullptr;

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_analytics_WebBrowserWrapper_initNativeBrowser(
    JNIEnv *env,
    jobject objInst)
{
    if (WebBrowserNative != nullptr)
    {
        return;
    }

    env->GetJavaVM(&CurrJavaVM);

    jclass objCls = env->GetObjectClass(objInst);
    WebBrowserObjInst = env->NewGlobalRef(objInst);
    WebBrowserLoadContent = env->GetMethodID(objCls, "loadContent", "(Ljava/lang/String;Ljava/lang/String;)V");
    WebBrowserSendEvent = env->GetMethodID(objCls, "sendEvent", "(Ljava/lang/String;Ljava/lang/String;)V");

    WebBrowserNative = WebBrowser::GetInstance();
}

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace analytics {
void WebBrowserBinding::LoadContent(std::string webPageContent) {
    JNIEnv *env = nullptr;
    CurrJavaVM->AttachCurrentThread(&env, nullptr);
    jstring content = env->NewStringUTF(webPageContent.c_str());

    env->CallVoidMethod(WebBrowserObjInst, WebBrowserLoadContent, content);
}

void WebBrowserBinding::SendEvent(std::string evtName, std::string evtData) {
    JNIEnv *env = nullptr;
    CurrJavaVM->AttachCurrentThread(&env, nullptr);
    jstring evtNameVM = env->NewStringUTF(evtName.c_str());
    jstring evtDataVM = env->NewStringUTF(evtData.c_str());

    env->CallVoidMethod(WebBrowserObjInst, WebBrowserSendEvent, evtNameVM,
                        evtDataVM);
}
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace analytics {
std::shared_ptr<WebBrowser> WebBrowser::GetInstance()
{
    return std::shared_ptr<WebBrowser>(new WebBrowserBinding);
}
}
}
}
