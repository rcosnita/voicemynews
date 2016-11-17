#include "RequireBinding.h"
#include "io/fs/Require.h"
#include "io/fs/FileUtilsPlatform.h"

#include <codecvt>
#include <memory>
#include <iostream>
#include <string>

using namespace v8;

using voicemynews::core::io::fs::FileUtilsPlatform;
using voicemynews::core::io::fs::Require;

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace require {
void LoadRequire(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto isolate = info.GetIsolate();
    std::shared_ptr<FileUtilsPlatform> fileUtils = std::make_shared<FileUtilsPlatform>();
    Require require(fileUtils);

    Local<String> moduleName = info[0]->ToString();
    String::Utf8Value moduleNameUtf8(moduleName);
    std::string fileName = *moduleNameUtf8;

    auto sourceWide = require.Load(fileName);
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string source = converter.to_bytes(sourceWide);

    auto fnResult = String::NewFromUtf8(isolate, source.c_str(), NewStringType::kNormal).ToLocalChecked();
    info.GetReturnValue().Set(fnResult);
}

void LoadRawRequire(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto isolate = info.GetIsolate();
    std::shared_ptr<FileUtilsPlatform> fileUtils = std::make_shared<FileUtilsPlatform>();
    Require require(fileUtils);

    Local<String> moduleName = info[0]->ToString();
    String::Utf8Value moduleNameUtf8(moduleName);
    std::string fileName = *moduleNameUtf8;

    auto sourceWide = require.LoadRaw(fileName);
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string source = converter.to_bytes(sourceWide);

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