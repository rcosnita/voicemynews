#include "VoiceBinding.h"
#include "bindings/events/EventLoopBinding.h"

#include <exception>
#include <functional>
#include <memory>
#include <bindings/events/EventLoopPlatform.h>

using namespace v8;

static JavaVM* CurrJavaVM = nullptr;

static const int kInternalFieldWhenProgress = 1;
static const int kInternalFieldWhenReadPaused = 2;
static const int kInternalFieldWhenReadResumed = 3;
static const int kInternalFieldWhenDone = 4;

static jobject TtsEngine = nullptr;
static jmethodID TtsEngineReadText = nullptr;
static jmethodID TtsEngineReadSsml = nullptr;
static jmethodID TtsEnginePause = nullptr;
static jmethodID TtsEngineResume = nullptr;
static jmethodID TtsEngineSkip = nullptr;

static jclass VoiceSupportActionsCls = nullptr;
static jmethodID VoiceSupportActionsCtor = nullptr;
static jmethodID VoiceSupportActionsOnPlayheadChanged = nullptr;
static jmethodID VoiceSupportActionsOnPaused = nullptr;
static jmethodID VoiceSupportActionsOnResumed = nullptr;
static jmethodID VoiceSupportActionsOnDone = nullptr;

using VoiceSupportAbstract = voicemynews::core::voice::VoiceSupportAbstract;
using VoiceBinding = voicemynews::app::android::bindings::news::VoiceBinding;
using VoiceReadingNotifications = voicemynews::app::android::bindings::news::VoiceBinding::VoiceReadingNotifications;

template<typename T>
class SimpleCarry
{
private:
    T data_;
public:
    SimpleCarry(T&& data) : data_(data) { }

    T Data()
    {
        return data_;
    }
};

/**
 * \brief Provides a js wrapper over ReadText voice binding implementation.
 */
static void ReadJsVoiceSupportText(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input.
    auto text = std::string(*String::Utf8Value(info[0]->ToString()));
    auto notificationsJs = info[1]->ToObject();
    auto holder = info.Holder();

    auto voiceSupportPtr = Local<External>::Cast(holder->GetInternalField(0))->Value();
    auto voiceSupportCarry = reinterpret_cast<SimpleCarry<std::shared_ptr<VoiceSupportAbstract>>*>(voiceSupportPtr);
    auto voiceSupport = voiceSupportCarry->Data();
    auto notificationsPtr = Local<External>::Cast(notificationsJs->GetInternalField(0))->Value();
    auto notificationsCarry = reinterpret_cast<SimpleCarry<std::shared_ptr<VoiceReadingNotifications>>*>(notificationsPtr);
    auto notifications = notificationsCarry->Data();

    voiceSupport->ReadText(text, notifications);
}

/**
 * \brief Provides a js wrapper over ReadSsml voice binding implementation.
 */
static void ReadJsVoiceSupportSsml(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input.
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
}

/**
 * \brief Provides a js wrapper over Pause voice binding implementation.
 */
static void PauseJsVoiceSupport(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input.
    auto notificationsJs = info[0]->ToObject();
    auto holder = info.Holder();

    auto voiceSupportPtr = Local<External>::Cast(holder->GetInternalField(0))->Value();
    auto voiceSupportCarry = reinterpret_cast<SimpleCarry<std::shared_ptr<VoiceSupportAbstract>>*>(voiceSupportPtr);
    auto voiceSupport = voiceSupportCarry->Data();
    auto notificationsPtr = Local<External>::Cast(notificationsJs->GetInternalField(0))->Value();
    auto notificationsCarry = reinterpret_cast<SimpleCarry<std::shared_ptr<VoiceReadingNotifications>>*>(notificationsPtr);
    auto notifications = notificationsCarry->Data();

    voiceSupport->Pause(notifications);
}

/**
 * \brief Provides a js wrapper over Resume voice binding implementation.
 */
static void ResumeJsVoiceSupport(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input.
    auto notificationsJs = info[0]->ToObject();
    auto holder = info.Holder();

    auto voiceSupportPtr = Local<External>::Cast(holder->GetInternalField(0))->Value();
    auto voiceSupportCarry = reinterpret_cast<SimpleCarry<std::shared_ptr<VoiceSupportAbstract>>*>(voiceSupportPtr);
    auto voiceSupport = voiceSupportCarry->Data();
    auto notificationsPtr = Local<External>::Cast(notificationsJs->GetInternalField(0))->Value();
    auto notificationsCarry = reinterpret_cast<SimpleCarry<std::shared_ptr<VoiceReadingNotifications>>*>(notificationsPtr);
    auto notifications = notificationsCarry->Data();

    voiceSupport->Resume(notifications);
}

/**
 * \brief Provides a js wrapper over Skip voice binding implementation.
 */
static void SkipJsVoiceSupport(const FunctionCallbackInfo<Value>& info)
{
    auto holder = info.Holder();

    // TODO [rcosnita] validate input.
    auto voiceSupportPtr = Local<External>::Cast(holder->GetInternalField(0))->Value();
    auto voiceSupportCarry = reinterpret_cast<SimpleCarry<std::shared_ptr<VoiceSupportAbstract>>*>(voiceSupportPtr);
    auto voiceSupport = voiceSupportCarry->Data();

    voiceSupport->Skip();
}


/**
 * \brief Provides a js wrapper which can instantiate VoiceSupport objects backed up by native instances.
 */
static void GetJsVoiceSupportInstance(const FunctionCallbackInfo<Value>& info)
{
    auto isolate = info.GetIsolate();

    auto obj = ObjectTemplate::New(isolate);
    obj->SetInternalFieldCount(1);
    obj->Set(isolate, "readText", FunctionTemplate::New(isolate, ReadJsVoiceSupportText));
    obj->Set(isolate, "readSsml", FunctionTemplate::New(isolate, ReadJsVoiceSupportSsml));
    obj->Set(isolate, "pause", FunctionTemplate::New(isolate, PauseJsVoiceSupport));
    obj->Set(isolate, "resume", FunctionTemplate::New(isolate, ResumeJsVoiceSupport));
    obj->Set(isolate, "skip", FunctionTemplate::New(isolate, SkipJsVoiceSupport));

    auto objInst = obj->NewInstance();
    objInst->SetInternalField(0, External::New(isolate, new SimpleCarry<std::shared_ptr<VoiceSupportAbstract>>(voicemynews::core::voice::GetVoiceSupportInstance())));

    info.GetReturnValue().Set(objInst);
}

/**
 * \brief Provides the getter method for retrieving whenDone callback.
 */
static void GetJsVoiceSupportNotificationsWhenDone(Local<String> property,
                                                   const PropertyCallbackInfo<Value>& info)
{
    auto isolate = info.GetIsolate();
    auto holder = info.Holder();
    auto whenDone = static_cast<Persistent<Function>*>(Local<External>::Cast(holder->GetInternalField(kInternalFieldWhenDone))->Value());

    info.GetReturnValue().Set(whenDone->Get(isolate));
}

/**
 * \brief Provides the getter method for retrieving whenPaused callback.
 */
static void GetJsVoiceSupportNotificationsWhenPaused(Local<String> property,
                                                   const PropertyCallbackInfo<Value>& info)
{
    auto isolate = info.GetIsolate();
    auto holder = info.Holder();
    auto whenPaused = static_cast<Persistent<Function>*>(Local<External>::Cast(holder->GetInternalField(kInternalFieldWhenReadPaused))->Value());

    info.GetReturnValue().Set(whenPaused->Get(isolate));
}

/**
 * \brief Provides the getter method for retrieving whenProgress callback.
 */
static void GetJsVoiceSupportNotificationsWhenProgress(Local<String> property,
                                                     const PropertyCallbackInfo<Value>& info)
{
    auto isolate = info.GetIsolate();
    auto holder = info.Holder();
    auto whenProgress = static_cast<Persistent<Function>*>(Local<External>::Cast(holder->GetInternalField(kInternalFieldWhenProgress))->Value());

    info.GetReturnValue().Set(whenProgress->Get(isolate));
}

/**
 * \brief Provides the getter method for retrieving whenResumed callback.
 */
static void GetJsVoiceSupportNotificationsWhenResumed(Local<String> property,
                                                       const PropertyCallbackInfo<Value>& info)
{
    auto isolate = info.GetIsolate();
    auto holder = info.Holder();
    auto whenResumed = static_cast<Persistent<Function>*>(Local<External>::Cast(holder->GetInternalField(kInternalFieldWhenReadResumed))->Value());

    info.GetReturnValue().Set(whenResumed->Get(isolate));
}

/**
 * \brief Provides the js wrapper which can build an object which holds all native reading notifications.
 *
 * This is extremely important, in order to be able to obtain notifications about progress, pause,
 * resume and done.
 */
static void GetJsVoiceSupportNotificationsInstance(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input parameters.
    auto isolate = info.GetIsolate();

    auto whenProgressLocal = Local<Function>::Cast(info[0]);
    auto whenProgress = new Persistent<Function>(isolate, whenProgressLocal);
    auto whenReadPausedLocal = Local<Function>::Cast(info[1]);
    auto whenReadPaused = new Persistent<Function>(isolate, whenReadPausedLocal);
    auto whenReadResumedLocal = Local<Function>::Cast(info[2]);
    auto whenReadResumed = new Persistent<Function>(isolate, whenReadResumedLocal);
    auto whenDoneLocal = Local<Function>::Cast(info[3]);
    auto whenDone = new Persistent<Function>(isolate, whenDoneLocal);

    auto obj = ObjectTemplate::New(isolate);
    obj->SetAccessor(String::NewFromUtf8(isolate, "whenDone"), GetJsVoiceSupportNotificationsWhenDone);
    obj->SetAccessor(String::NewFromUtf8(isolate, "whenPaused"), GetJsVoiceSupportNotificationsWhenPaused);
    obj->SetAccessor(String::NewFromUtf8(isolate, "whenProgress"), GetJsVoiceSupportNotificationsWhenProgress);
    obj->SetAccessor(String::NewFromUtf8(isolate, "whenResumed"), GetJsVoiceSupportNotificationsWhenResumed);

    obj->SetInternalFieldCount(5);

    auto objInst = obj->NewInstance();

    objInst->SetInternalField(0, External::New(isolate, new SimpleCarry<std::shared_ptr<VoiceReadingNotifications>>(
        std::make_shared<VoiceReadingNotifications>(
            std::function<void(int)>([whenProgress, isolate](int currPos) {
                auto whenProgressLocal = whenProgress->Get(isolate);
                Local<Value> args[1];
                args[0] = Number::New(isolate, currPos);

                whenProgressLocal->CallAsFunction(whenProgressLocal, 1, args);
            }),
            std::function<void(long)>([whenReadPaused, isolate](long currPos) {
                auto whenReadPausedLocal = whenReadPaused->Get(isolate);
                Local<Value> args[1];
                args[0] = Number::New(isolate, currPos);

                whenReadPausedLocal->CallAsFunction(whenReadPausedLocal, 1, args);
            }),
            std::function<void(long)>([whenReadResumed, isolate](long currPos) {
                auto whenReadResumedLocal = whenReadResumed->Get(isolate);
                Local<Value> args[1];
                args[0] = Number::New(isolate, currPos);

                whenReadResumedLocal->CallAsFunction(whenReadResumedLocal, 1, args);
            }),
            std::function<void()>([whenDone, isolate]() {
                auto whenDoneLocal = whenDone->Get(isolate);
                Local<Value> args[0];

                whenDoneLocal->CallAsFunction(whenDoneLocal, 0, args);
            })
        )
    )));
    objInst->SetInternalField(kInternalFieldWhenProgress, External::New(isolate, whenProgress));
    objInst->SetInternalField(kInternalFieldWhenReadPaused, External::New(isolate, whenReadPaused));
    objInst->SetInternalField(kInternalFieldWhenReadResumed, External::New(isolate, whenReadResumed));
    objInst->SetInternalField(kInternalFieldWhenDone, External::New(isolate, whenDone));

    info.GetReturnValue().Set(objInst);
}

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace news {
void InitVoiceSupportBinding(JNIEnv* env, jobject voiceSupportInstance)
{
    env->GetJavaVM(&CurrJavaVM);
    TtsEngine = env->NewGlobalRef(voiceSupportInstance);

    auto ttsEngineCls = env->GetObjectClass(voiceSupportInstance);
    TtsEngineReadText = env->GetMethodID(ttsEngineCls, "readText", "(Ljava/lang/String;Lcom/voicemynews/core/bindings/news/VoiceSupportActions;)V");
    TtsEngineReadSsml = env->GetMethodID(ttsEngineCls, "readSsml", "(Ljava/lang/String;Lcom/voicemynews/core/bindings/news/VoiceSupportActions;)V");
    TtsEnginePause = env->GetMethodID(ttsEngineCls, "pause", "(Lcom/voicemynews/core/bindings/news/VoiceSupportActions;)V");
    TtsEngineResume = env->GetMethodID(ttsEngineCls, "resume", "(Lcom/voicemynews/core/bindings/news/VoiceSupportActions;)V");
    TtsEngineSkip = env->GetMethodID(ttsEngineCls, "skip", "()V");

    VoiceSupportActionsCls = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("com/voicemynews/core/bindings/news/VoiceSupportActions")));
    VoiceSupportActionsCtor = env->GetMethodID(VoiceSupportActionsCls, "<init>", "(J)V");
    VoiceSupportActionsOnPlayheadChanged = env->GetMethodID(VoiceSupportActionsCls, "onPlayheadChanged", "(I)V");
    VoiceSupportActionsOnPaused = env->GetMethodID(VoiceSupportActionsCls, "onPaused", "(J)V");
    VoiceSupportActionsOnResumed = env->GetMethodID(VoiceSupportActionsCls, "onPaused", "(J)V");
    VoiceSupportActionsOnDone = env->GetMethodID(VoiceSupportActionsCls, "onDone", "()V");
}

void VoiceBinding::WireToJs(v8::Isolate *isolate, v8::Local<v8::ObjectTemplate> obj)
{
    Local<ObjectTemplate> voiceSupport = ObjectTemplate::New(isolate);
    obj->Set(isolate, "VoiceSupport", voiceSupport);

    voiceSupport->Set(isolate, "getInstance", FunctionTemplate::New(isolate, GetJsVoiceSupportInstance));
    voiceSupport->Set(isolate, "getNotificationsInstance",
                      FunctionTemplate::New(isolate, GetJsVoiceSupportNotificationsInstance));
}

void VoiceBinding::ReadText(std::string text, std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
{
    JNIEnv* env = nullptr;
    CurrJavaVM->AttachCurrentThread(&env, nullptr);

    jlong notificationsPtr = reinterpret_cast<uintptr_t>(readingCallbacks.get());
    jstring textJVM = env->NewStringUTF(text.c_str());
    jobject actions = env->NewObject(VoiceSupportActionsCls, VoiceSupportActionsCtor, notificationsPtr);

    env->CallVoidMethod(TtsEngine, TtsEngineReadText, textJVM, actions);
}

void VoiceBinding::ReadSsml(std::string text, std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
{
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
}

void VoiceBinding::Pause(std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
{
    JNIEnv* env = nullptr;
    CurrJavaVM->AttachCurrentThread(&env, nullptr);

    jlong notificationsPtr = reinterpret_cast<uintptr_t>(readingCallbacks.get());
    jobject actions = env->NewObject(VoiceSupportActionsCls, VoiceSupportActionsCtor, notificationsPtr);

    env->CallVoidMethod(TtsEngine, TtsEnginePause, actions);
}

void VoiceBinding::Resume(std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
{
    JNIEnv* env = nullptr;
    CurrJavaVM->AttachCurrentThread(&env, nullptr);

    jlong notificationsPtr = reinterpret_cast<uintptr_t>(readingCallbacks.get());
    jobject actions = env->NewObject(VoiceSupportActionsCls, VoiceSupportActionsCtor, notificationsPtr);

    env->CallVoidMethod(TtsEngine, TtsEngineResume, actions);
}

void VoiceBinding::Skip()
{
    JNIEnv* env = nullptr;
    CurrJavaVM->AttachCurrentThread(&env, nullptr);

    jobject actions = env->NewObject(VoiceSupportActionsCls, VoiceSupportActionsCtor, nullptr);

    env->CallVoidMethod(TtsEngine, TtsEngineSkip, actions);
}
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace voice {
/**
 * \brief Provides a factory method for obtaining voice support engine instance.
 */
std::shared_ptr<VoiceSupportAbstract> GetVoiceSupportInstance()
{
    return std::make_shared<VoiceBinding>();
}
}
}
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_news_VoiceSupportActions_invokeWhenPlayheadChangedFn(
    JNIEnv* env,
    jobject objInst,
    jint pos,
    jlong fnPtr)
{
    auto fn = reinterpret_cast<VoiceReadingNotifications*>(fnPtr);
    auto evtLoop = voicemynews::core::events::EventLoopPlatform::GetInstance();
    evtLoop->EnqueueTask([fn, pos]() {
        // TODO [rcosnita] uncomment this line when you want to start telling the progress of the playhead.
        // fn->whenPlayheadChanged()(pos);
    });
}

/**
 * \brief Provides the implementation of com.voicemynews.core.bindings.news.VoiceSupportActions.invokeWhenPausedFn.
 *
 * Internally it invokes the specified fnPtr and passes to it the position received as argument.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_news_VoiceSupportActions_invokeWhenPausedFn(
    JNIEnv* env,
    jobject objInst,
    jlong pos,
    jlong fnPtr)
{
    auto fn = reinterpret_cast<VoiceReadingNotifications*>(fnPtr);
    auto evtLoop = voicemynews::core::events::EventLoopPlatform::GetInstance();
    evtLoop->EnqueueTask([fn, pos]() {
        fn->whenPaused()(pos);
    });
}

/**
 * \brief Provides the implementation of com.voicemynews.core.bindings.news.VoiceSupportActions.invokeWhenResumedFn.
 *
 * Internally it invokes the specified fnPtr and passes to it the position received as argument.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_news_VoiceSupportActions_invokeWhenResumedFn(
    JNIEnv* env,
    jobject objInst,
    jlong pos,
    jlong fnPtr)

{
    auto fn = reinterpret_cast<VoiceReadingNotifications*>(fnPtr);
    auto evtLoop = voicemynews::core::events::EventLoopPlatform::GetInstance();
    evtLoop->EnqueueTask([fn, pos]() {
        fn->whenResumed()(pos);
    });
}

/**
 * \brief Provides the implementation of com.voicemynews.core.bindings.news.VoiceSupportActions.invokeWhenDoneFn.
 *
 * Internally it invokes the specified fnPtr.
 */
JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_news_VoiceSupportActions_invokeWhenDoneFn(
    JNIEnv* env,
    jobject objInst,
    jlong fnPtr)
{
    auto fn = reinterpret_cast<VoiceReadingNotifications*>(fnPtr);

    auto evtLoop = voicemynews::core::events::EventLoopPlatform::GetInstance();
    evtLoop->EnqueueTask([fn]() {
        fn->whenDone()();
    });
}
