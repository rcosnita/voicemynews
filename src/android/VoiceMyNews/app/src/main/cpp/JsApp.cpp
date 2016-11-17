#include "JsApp.h"
#include "io/fs/Require.h"
#include "io/fs/FileUtilsPlatform.h"

#include <android/asset_manager_jni.h>
#include <codecvt>
#include <memory>

using namespace v8;

using voicemynews::core::io::fs::FileUtilsPlatform;
using voicemynews::core::io::fs::Require;

static Isolate::CreateParams createParams_ = Isolate::CreateParams();
static Platform* V8Platform = nullptr;

static voicemynews::app::android::js::JsApp* JsAppCurrInstance = nullptr;

JNIEXPORT void JNICALL Java_com_voicemynews_voicemynews_JsApp_startPlatform(
    JNIEnv* env,
    jclass objClass,
    jlong nativeEmitterPtr,
    jobject assetManager)
{
    assetManager = env->NewGlobalRef(assetManager);
    voicemynews::core::io::fs::FileUtilsPlatform::Initialize(AAssetManager_fromJava(env, assetManager));
    auto eventLoop = reinterpret_cast<voicemynews::app::android::js::JsApp::EventLoopPlatform*>(nativeEmitterPtr);
    voicemynews::app::android::js::JsApp::StartPlatform();

    auto jsApp = voicemynews::app::android::js::JsApp::GetInstance(eventLoop);
    jsApp->Start();
}

JNIEXPORT void JNICALL Java_com_voicemynews_voicemynews_JsApp_shutdownPlatform(
    JNIEnv* env,
    jclass objClass)
{
    voicemynews::app::android::js::JsApp::ShutdownPlatform();
}

/**
 * \brief Provides a thin wrapper over load method from require.
 *
 * It can be used from javascript.
 */
static void LoadRequire(const FunctionCallbackInfo<Value>& info)
{
    auto isolate = info.GetIsolate();
    std::shared_ptr<FileUtilsPlatform> fileUtils = std::make_shared<FileUtilsPlatform>();
    std::shared_ptr<Require> require = std::make_shared<Require>(fileUtils);

    Local<String> moduleName = info[0]->ToString();
    String::Utf8Value moduleNameUtf8(moduleName);
    std::string fileName = *moduleNameUtf8;

    auto sourceWide = require->Load(fileName);
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string source = converter.to_bytes(sourceWide);

    auto fnResult = String::NewFromUtf8(isolate, source.c_str(), NewStringType::kNormal).ToLocalChecked();
    info.GetReturnValue().Set(fnResult);
}

/**
 * \brief Provides a thin wrapper over loadRaw method from require.
 *
 * It can be used from javascript.
 */
static void LoadRawRequire(const FunctionCallbackInfo<Value>& info)
{
    throw std::exception();
}

/**
 * \brief Provides the algorithm for obtaining a new instance of require native object.
 */
static void GetRequireInstance(const FunctionCallbackInfo<Value>& info)
{
    auto isolate = info.GetIsolate();

    Local<ObjectTemplate> requireBinding = ObjectTemplate::New(isolate);
    requireBinding->Set(isolate, "load", FunctionTemplate::New(isolate, LoadRequire));
    requireBinding->Set(isolate, "loadRaw", FunctionTemplate::New(isolate, LoadRawRequire));

    info.GetReturnValue().Set(requireBinding->NewInstance());
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

    global->Set(isolate_, "global", ObjectTemplate::New(isolate_));
    global->Set(isolate_, "voicemynews", voicemynewsLocal);

    voicemynewsLocal->Set(isolate_, "core", voicemynewsCoreLocal);

    voicemynewsObj = new Persistent<ObjectTemplate>(isolate_, voicemynewsLocal);
    voicemynewsCoreObj = new Persistent<ObjectTemplate>(isolate_, voicemynewsCoreLocal);

    BindRequireJsNativeSupport();

    auto context_ = Context::New(isolate_, nullptr, global);
    persistentContext_ = new Persistent<Context>(isolate_, context_);
    contextScope_ = new Context::Scope(context_);

    BindRequireJsSupport();
    BindHttpClientSupport();
    BindNavigationManagerSupport();
    BindVoiceSupport();

    StartApp();
}

void JsApp::BindRequireJsNativeSupport()
{
    Isolate::Scope isolateScope(isolate_);
    HandleScope handleScope(isolate_);
    Local<ObjectTemplate> voicemynewsCore = voicemynewsCoreObj->Get(isolate_);
    Local<ObjectTemplate> requireFactory = ObjectTemplate::New(isolate_);

    requireFactory->Set(isolate_, "getInstance", FunctionTemplate::New(isolate_, GetRequireInstance));
    voicemynewsCore->Set(isolate_, "RequireFactory", requireFactory);
}

void JsApp::BindRequireJsSupport()
{
    auto requireSource = fileUtils_.ReadFilePlatform("js/require.js");
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string requireSourceUtf8 = converter.to_bytes(requireSource);

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
}

void JsApp::BindNavigationManagerSupport()
{
}

void JsApp::BindVoiceSupport()
{
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
