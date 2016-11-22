#include "RequireBinding.h"
#include "io/fs/Require.h"
#include "io/fs/FileUtilsPlatform.h"

#include <memory>
#include <iostream>
#include <string>

using namespace v8;

using voicemynews::core::io::fs::FileUtilsPlatform;
using voicemynews::core::io::fs::Require;

static std::string* LastLoadedModuleName = nullptr;
static std::string* LastLoadedRawFileName = nullptr;

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace require {
std::string GetLastLoadedModuleName()
{
    if (LastLoadedModuleName != nullptr)
    {
        return *LastLoadedModuleName;
    }

    return std::string("");
}

std::string GetLastLoadedRawFileName()
{
    if (LastLoadedRawFileName != nullptr)
    {
        return *LastLoadedRawFileName;
    }

    return std::string("");
}

void LoadRequire(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    // TODO [rcosnita] validate js input parameters.
    auto isolate = info.GetIsolate();
    std::shared_ptr<FileUtilsPlatform> fileUtils = std::make_shared<FileUtilsPlatform>();
    Require require(fileUtils);

    Local<String> moduleName = info[0]->ToString();
    String::Utf8Value moduleNameUtf8(moduleName);
    std::string fileName = *moduleNameUtf8;

    if (LastLoadedModuleName != nullptr) {
        delete LastLoadedModuleName;
    }
    LastLoadedModuleName = new std::string(fileName);

    auto sourceWide = require.Load(fileName);
    std::string source(sourceWide.begin(), sourceWide.end());

    auto fnResult = String::NewFromUtf8(isolate, source.c_str(), NewStringType::kNormal).ToLocalChecked();
    info.GetReturnValue().Set(fnResult);
}

void LoadRawRequire(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    // TODO [rcosnita] validate js input parameters.
    auto isolate = info.GetIsolate();
    std::shared_ptr<FileUtilsPlatform> fileUtils = std::make_shared<FileUtilsPlatform>();
    Require require(fileUtils);

    Local<String> moduleName = info[0]->ToString();
    String::Utf8Value moduleNameUtf8(moduleName);
    std::string fileName = *moduleNameUtf8;

    if (LastLoadedRawFileName != nullptr) {
        delete LastLoadedRawFileName;
    }
    LastLoadedRawFileName = new std::string(fileName);

    auto sourceWide = require.LoadRaw(fileName);
    std::string source(sourceWide.begin(), sourceWide.end());

    auto fnResult = String::NewFromUtf8(isolate, source.c_str(), NewStringType::kNormal).ToLocalChecked();
    info.GetReturnValue().Set(fnResult);
}

void GetRequireInstance(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto isolate = info.GetIsolate();

    Local<ObjectTemplate> requireBinding = ObjectTemplate::New(isolate);
    requireBinding->Set(isolate, "load", FunctionTemplate::New(isolate, LoadRequire));
    requireBinding->Set(isolate, "loadRaw", FunctionTemplate::New(isolate, LoadRawRequire));

    info.GetReturnValue().Set(requireBinding->NewInstance());
}
}
}
}
}
}