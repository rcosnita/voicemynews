#include "VoiceBinding.h"

#include <exception>
#include <functional>
#include <memory>

using namespace v8;

static const int kInternalFieldWhenProgress = 1;
static const int kInternalFieldWhenReadPaused = 2;
static const int kInternalFieldWhenReadResumed = 3;
static const int kInternalFieldWhenDone = 4;

/**
 * \brief Provides a js wrapper over ReadText voice binding implementation.
 */
static void ReadJsVoiceSupportText(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input.
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
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
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
}

/**
 * \brief Provides a js wrapper over Resume voice binding implementation.
 */
static void ResumeJsVoiceSupport(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input.
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
}

/**
 * \brief Provides a js wrapper over Skip voice binding implementation.
 */
static void SkipJsVoiceSupport(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input.
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
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
    objInst->SetInternalField(0, External::New(isolate, voicemynews::core::voice::GetVoiceSupportInstance().get()));

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
    auto readingNotifications = new voicemynews::app::android::bindings::news::VoiceBinding::VoiceReadingNotifications(
        std::function<void(int)>([&whenProgress, &isolate](int currPos) {
            auto whenProgressLocal = whenProgress->Get(isolate);
            Local<Value> args[1];
            args[0] = Number::New(isolate, currPos);

            whenProgressLocal->CallAsFunction(whenProgressLocal, 1, args);
        }),
        std::function<void(long)>([&whenReadPaused, &isolate](long currPos) {
            auto whenReadPausedLocal = whenReadPaused->Get(isolate);
            Local<Value> args[1];
            args[0] = Number::New(isolate, currPos);

            whenReadPausedLocal->CallAsFunction(whenReadPausedLocal, 1, args);
        }),
        std::function<void(long)>([&whenReadResumed, &isolate](long currPos) {
            auto whenReadResumedLocal = whenReadResumed->Get(isolate);
            Local<Value> args[1];
            args[0] = Number::New(isolate, currPos);

            whenReadResumedLocal->CallAsFunction(whenReadResumedLocal, 1, args);
        }),
        std::function<void()>([&whenDone, &isolate]() {
            auto whenDoneLocal = whenDone->Get(isolate);
            Local<Value> args[0];

            whenDoneLocal->CallAsFunction(whenDoneLocal, 1, args);
        })
    );

    objInst->SetInternalField(0, External::New(isolate, readingNotifications));
    objInst->SetInternalField(kInternalFieldWhenProgress, External::New(isolate, whenProgress));
    objInst->SetInternalField(kInternalFieldWhenReadPaused, External::New(isolate, whenReadPaused));
    objInst->SetInternalField(kInternalFieldWhenReadResumed, External::New(isolate, whenReadResumed));
    objInst->SetInternalField(kInternalFieldWhenDone, External::New(isolate, whenDone));
}

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace news {
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
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
}

void VoiceBinding::ReadSsml(std::string text, std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
{
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
}

void VoiceBinding::Pause(std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
{
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
}

void VoiceBinding::Resume(std::shared_ptr<VoiceReadingNotifications> readingCallbacks)
{
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
}

void VoiceBinding::Skip()
{
    // TODO [rcosnita] implement this when necessary.
    throw std::exception();
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
    return std::shared_ptr<VoiceSupportAbstract>(new voicemynews::app::android::bindings::news::VoiceBinding);
}
}
}
}