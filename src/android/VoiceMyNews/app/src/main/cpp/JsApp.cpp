#include "JsApp.h"
#include "bindings/events/NavigationBinding.h"
#include "bindings/network/HttpClientBinding.h"
#include "bindings/news/VoiceBinding.h"

#include <android/asset_manager_jni.h>
#include <thread>

using namespace v8;

static Isolate::CreateParams createParams_ = Isolate::CreateParams();
static Platform* V8Platform = nullptr;

static voicemynews::app::android::js::JsApp* JsAppCurrInstance = nullptr;

static std::thread* V8Thread = nullptr;
static JavaVM* CurrJavaVM = nullptr;

JNIEXPORT void JNICALL Java_com_voicemynews_voicemynews_JsApp_startPlatform(
    JNIEnv* env,
    jclass objClass,
    jlong nativeEmitterPtr,
    jobject assetManager)
{
    if (V8Thread != nullptr) {
        return;
    }

    env->GetJavaVM(&CurrJavaVM);

    assetManager = env->NewGlobalRef(assetManager);
    V8Thread = new std::thread([assetManager, nativeEmitterPtr]() {
        JNIEnv* env = nullptr;
        CurrJavaVM->AttachCurrentThread(&env, nullptr);
        voicemynews::core::io::fs::FileUtilsPlatform::Initialize(AAssetManager_fromJava(env, assetManager));
        auto eventLoop = reinterpret_cast<voicemynews::app::android::js::JsApp::EventLoopPlatform*>(nativeEmitterPtr);
        voicemynews::app::android::js::JsApp::StartPlatform();

        auto jsApp = voicemynews::app::android::js::JsApp::GetInstance(eventLoop);
        jsApp->Start();
    });
}

JNIEXPORT void JNICALL Java_com_voicemynews_voicemynews_JsApp_shutdownPlatform(
    JNIEnv* env,
    jclass objClass)
{
    voicemynews::app::android::js::JsApp::ShutdownPlatform();
}

namespace voicemynews {
namespace app {
namespace android {
namespace js {
JNIEXPORT void JNICALL Java_com_voicemynews_voicemynews_startPlatform(
    JNIEnv* env,
    jclass objClass)
{
    JsApp::StartPlatform();
}

void JsApp::StartPlatform()
{
    V8::InitializeICUDefaultLocation("./");
    V8::InitializeExternalStartupData("./");
    V8Platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(V8Platform);
    V8::Initialize();

    createParams_.array_buffer_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
}

void JsApp::ShutdownPlatform()
{
    V8::Dispose();
    V8::ShutdownPlatform();

    delete V8Platform;
    delete createParams_.array_buffer_allocator;
}

JsApp::JsApp(JsApp::EventLoopPlatform* eventLoop)
    : eventLoop_(eventLoop)
{
}

JsApp::~JsApp()
{
    persistentContext_->Reset();
    isolate_->Dispose();

    contextScope_ = nullptr;
    persistentContext_ = nullptr;
    isolate_ = nullptr;
}

JsApp* JsApp::GetInstance(JsApp::EventLoopPlatform* eventLoop)
{
    if (JsAppCurrInstance == nullptr)
    {
        if (eventLoop == nullptr)
        {
            eventLoop = JsApp::EventLoopPlatform::GetInstance();
        }

        JsAppCurrInstance = new JsApp(eventLoop);
    }

    return JsAppCurrInstance;
}

void JsApp::Start()
{
    isolate_ = Isolate::New(createParams_);
    Isolate::Scope isolateScope(isolate_);
    HandleScope handleScope(isolate_);
    Local<ObjectTemplate> global = ObjectTemplate::New(isolate_);
    Local<ObjectTemplate> voicemynewsLocal = ObjectTemplate::New(isolate_);
    Local<ObjectTemplate> voicemynewsCoreLocal = ObjectTemplate::New(isolate_);
    Local<ObjectTemplate> voicemynewsEventsLocal = ObjectTemplate::New(isolate_);
    Local<ObjectTemplate> voicemynewsNetworkLocal = ObjectTemplate::New(isolate_);
    Local<ObjectTemplate> voicemynewsVoiceLocal = ObjectTemplate::New(isolate_);

    global->Set(isolate_, "global", ObjectTemplate::New(isolate_));
    global->Set(isolate_, "voicemynews", voicemynewsLocal);

    voicemynewsLocal->Set(isolate_, "core", voicemynewsCoreLocal);
    voicemynewsCoreLocal->Set(isolate_, "events", voicemynewsEventsLocal);
    voicemynewsCoreLocal->Set(isolate_, "network", voicemynewsNetworkLocal);
    voicemynewsCoreLocal->Set(isolate_, "voice", voicemynewsVoiceLocal);

    // TODO [rcosnita] cleanup all resources during destruction phase.
    voicemynewsObj = new Persistent<ObjectTemplate>(isolate_, voicemynewsLocal);
    voicemynewsCoreObj = new Persistent<ObjectTemplate>(isolate_, voicemynewsCoreLocal);
    voicemynewsEventsObj = new Persistent<ObjectTemplate>(isolate_, voicemynewsEventsLocal);
    voicemynewsNetworkObj = new Persistent<ObjectTemplate>(isolate_, voicemynewsNetworkLocal);
    voicemynewsVoiceObj = new Persistent<ObjectTemplate>(isolate_, voicemynewsVoiceLocal);

    BindRequireJsNativeSupport();
    BindEventPlatformSupport();
    BindHttpClientSupport();
    BindNavigationManagerSupport();
    BindVoiceSupport();

    auto context_ = Context::New(isolate_, nullptr, global);
    persistentContext_ = new Persistent<Context>(isolate_, context_);
    contextScope_ = new Context::Scope(context_);

    BindRequireJsSupport();
    StartApp();
}

void JsApp::BindRequireJsNativeSupport()
{
    Isolate::Scope isolateScope(isolate_);
    HandleScope handleScope(isolate_);
    Local<ObjectTemplate> voicemynewsCore = voicemynewsCoreObj->Get(isolate_);
    Local<ObjectTemplate> requireFactory = ObjectTemplate::New(isolate_);

    requireFactory->Set(isolate_, "getInstance", FunctionTemplate::New(isolate_, voicemynews::app::android::bindings::require::GetRequireInstance));
    voicemynewsCore->Set(isolate_, "RequireFactory", requireFactory);
}

void JsApp::BindEventPlatformSupport()
{
    Isolate::Scope isolateScope(isolate_);
    EscapableHandleScope handleScope(isolate_);
    Local<ObjectTemplate> voicemynewsEvents = voicemynewsEventsObj->Get(isolate_);

    EventLoopPlatform::WireToJs(isolate_, handleScope.Escape(voicemynewsEvents));
}

void JsApp::BindRequireJsSupport()
{
    auto requireSource = fileUtils_.ReadFilePlatform("js/require.js");
    std::string requireSourceUtf8(requireSource.begin(), requireSource.end());

    Isolate::Scope isolateScope(isolate_);
    HandleScope handleScope(isolate_);
    Local<Context> context = persistentContext_->Get(isolate_);

    TryCatch tryCatch(isolate_);
    Local<String> scriptSource = String::NewFromUtf8(isolate_, requireSourceUtf8.c_str(), NewStringType::kNormal).ToLocalChecked();
    MaybeLocal<Script> script = Script::Compile(context, scriptSource);

    HandleJsException(tryCatch, script);

    MaybeLocal<Value> result = script.ToLocalChecked()->Run(context);

    HandleJsException(tryCatch, result);
}

void JsApp::BindHttpClientSupport()
{
    Isolate::Scope isolateScope(isolate_);
    EscapableHandleScope handleScope(isolate_);
    Local<ObjectTemplate> voicemynewsNetworkLocal = voicemynewsNetworkObj->Get(isolate_);

    voicemynews::app::android::bindings::HttpClientBinding::WireToJs(isolate_, handleScope.Escape(voicemynewsNetworkLocal));
}

void JsApp::BindNavigationManagerSupport()
{
    Isolate::Scope isolateScope(isolate_);
    EscapableHandleScope handleScope(isolate_);
    Local<ObjectTemplate> voicemynewsEventsLocal = voicemynewsEventsObj->Get(isolate_);

    voicemynews::app::android::bindings::events::NavigationBinding::WireToJs(isolate_, handleScope.Escape(voicemynewsEventsLocal));
}

void JsApp::BindVoiceSupport()
{
    Isolate::Scope isolateScope(isolate_);
    EscapableHandleScope handleScope(isolate_);
    Local<ObjectTemplate> voicemynewsVoiceLocal = voicemynewsVoiceObj->Get(isolate_);

    voicemynews::app::android::bindings::news::VoiceBinding::WireToJs(isolate_, handleScope.Escape(voicemynewsVoiceLocal));
}

void JsApp::StartApp()
{
    std::string appScript = "(() => { const app = require('js/app'); })();";

    Isolate::Scope isolateScope(isolate_);
    HandleScope handleScope(isolate_);
    Local<Context> context = persistentContext_->Get(isolate_);

    TryCatch tryCatch(isolate_);
    Local<String> scriptSource = String::NewFromUtf8(isolate_, appScript.c_str(), NewStringType::kNormal).ToLocalChecked();
    Local<Script> script = Script::Compile(context, scriptSource).ToLocalChecked();
    MaybeLocal<Value> result = script->Run(context);

    HandleJsException(tryCatch, result);
}

JsApp::EventLoopPlatform* JsApp::GetEventLoop()
{
    return eventLoop_;
}
}
}
}
}
