// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "V8HelloWorld.h"
#include "libplatform/libplatform.h"
#include "v8.h"
using namespace v8;

static Isolate::CreateParams createParams_ = Isolate::CreateParams();
static Platform* platform_;


namespace voicemynews {
namespace app {
namespace android {
class V8WrapperHelper
{
private:
    Isolate* isolate_;
    Persistent<Context> *persistentContext_;
    Context::Scope *contextScope_;

public:
    V8WrapperHelper()
    {
        isolate_ = Isolate::New(createParams_);
        Isolate::Scope isolateScope(isolate_);
        HandleScope handleScope(isolate_);
        auto context_ = Context::New(isolate_);
        persistentContext_ = new Persistent<Context>(isolate_, context_);
        contextScope_ = new Context::Scope(context_);
    }

    char* SayHello()
    {
        Isolate::Scope isolateScope(isolate_);
        HandleScope handleScope(isolate_);
        Local<Context> context = persistentContext_->Get(isolate_);
        Local<String> source =
            String::NewFromUtf8(isolate_, "'Hello' + ', World!'",
                                NewStringType::kNormal).ToLocalChecked();
        Local<Script> script = Script::Compile(context, source).ToLocalChecked();
        Local<Value> result = script->Run(context).ToLocalChecked();

        String::Utf8Value utf8(result);
        auto tmp = *utf8;
        char* resultStr = new char[strlen(tmp) + 1];
        strncpy(resultStr, tmp, strlen(tmp) + 1);
        return resultStr;
    }

    virtual ~V8WrapperHelper()
    {
        persistentContext_->Reset();
        isolate_->Dispose();

        contextScope_ = nullptr;
        persistentContext_ = nullptr;
        isolate_ = nullptr;
    }

    static void Initialize()
    {
        V8::InitializeICUDefaultLocation("./");
        V8::InitializeExternalStartupData("./");
        platform_ = platform::CreateDefaultPlatform();
        V8::InitializePlatform(platform_);
        V8::Initialize();

        createParams_.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    }

    static void Shutdown()
    {
        V8::Dispose();
        V8::ShutdownPlatform();

        delete platform_;
        delete createParams_.array_buffer_allocator;
    }
};

char* RunV8HelloWorld()
{
    V8WrapperHelper::Initialize();
    auto wrapper = new V8WrapperHelper();
    char* resultStr = wrapper->SayHello();
    delete wrapper;
    V8WrapperHelper::Shutdown();

    return resultStr;
}
}
}
}
