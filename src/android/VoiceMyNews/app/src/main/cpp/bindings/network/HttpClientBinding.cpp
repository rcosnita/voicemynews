#include "HttpClientBinding.h"
#include "bindings/events/EventLoopPlatform.h"

using namespace v8;

static jobject HttpClientObj = nullptr;
static jmethodID HttpClientGetMethod = nullptr;
static jmethodID HttpClientParseResponseWithStringContent = nullptr;
static jmethodID HttpClientGetNewHeadersMap = nullptr;
static jmethodID HttpClientHeadersMapPut = nullptr;
static jmethodID HttpClientBindingGetActionConstructor = nullptr;
static jclass HttpClientBindingGetActionClass = nullptr;
static jfieldID HttpClientBindingGetActionJsCallbackPtr = nullptr;
static jfieldID HttpClientBindingGetActionIsolatePtr = nullptr;
static jclass HttpClientBindingParseStringContentActionClass = nullptr;
static jmethodID HttpClientBindingParseStringContentActionConstructor = nullptr;
static jfieldID HttpClientBindingParseStringContentActionJsCallbackPtr = nullptr;
static jfieldID HttpClientBindingParseStringContentActionIsolatePtr = nullptr;
static JavaVM* javaVM = nullptr;

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_network_HttpClientBinding_initializeNative(
    JNIEnv* env,
    jclass objClass,
    jobject httpClientObj)
{
    if (HttpClientObj != nullptr)
    {
        return;
    }

    env->GetJavaVM(&javaVM);
    HttpClientObj = env->NewGlobalRef(httpClientObj);
    HttpClientGetMethod = env->GetMethodID(objClass, "get", "(Ljava/lang/String;Ljava/util/Map;Lcom/voicemynews/core/bindings/network/HttpClientBindingGetAction;)V");
    HttpClientParseResponseWithStringContent = env->GetMethodID(objClass, "parseResponseWithStringContent", "(Ljava/lang/Object;Lcom/voicemynews/core/bindings/network/HttpClientBindingParseStringContentAction;)V");
    HttpClientGetNewHeadersMap = env->GetMethodID(objClass, "getNewHeadersMap", "()Ljava/util/Map;");

    auto mapClass = env->FindClass("java/util/HashMap");
    HttpClientHeadersMapPut = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

    HttpClientBindingGetActionClass = (jclass)env->NewGlobalRef(env->FindClass("com/voicemynews/core/bindings/network/HttpClientBindingGetAction"));
    HttpClientBindingGetActionConstructor = env->GetMethodID(HttpClientBindingGetActionClass, "<init>", "(JJ)V");
    HttpClientBindingGetActionJsCallbackPtr = env->GetFieldID(HttpClientBindingGetActionClass, "jsCallbackPtr", "J");
    HttpClientBindingGetActionIsolatePtr = env->GetFieldID(HttpClientBindingGetActionClass, "isolatePtr", "J");

    HttpClientBindingParseStringContentActionClass = (jclass)env->NewGlobalRef(env->FindClass("com/voicemynews/core/bindings/network/HttpClientBindingParseStringContentAction"));
    HttpClientBindingParseStringContentActionConstructor = env->GetMethodID(HttpClientBindingGetActionClass, "<init>", "(JJ)V");
    HttpClientBindingParseStringContentActionJsCallbackPtr = env->GetFieldID(HttpClientBindingGetActionClass, "jsCallbackPtr", "J");
    HttpClientBindingParseStringContentActionIsolatePtr = env->GetFieldID(HttpClientBindingGetActionClass, "isolatePtr", "J");
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_network_HttpClientBindingGetAction_invokeJsCallback(
    JNIEnv* env,
    jobject thisObj,
    jobject responseData)
{
    auto jsCallbackPtr = env->GetLongField(thisObj, HttpClientBindingGetActionJsCallbackPtr);
    auto isolatePtr = env->GetLongField(thisObj, HttpClientBindingGetActionIsolatePtr);
    auto isolate = reinterpret_cast<Isolate*>(isolatePtr);
    auto jsCallback = reinterpret_cast<Persistent<Function>*>(jsCallbackPtr);
    auto responseDataGlobal = env->NewGlobalRef(responseData);
    auto eventLoop = voicemynews::core::events::EventLoopPlatform::GetInstance();

    eventLoop->EnqueueTask(std::function<void()>([&jsCallback, &isolate, &responseDataGlobal]() {
        Local<Value> args[1];
        Local<ObjectTemplate> obj = ObjectTemplate::New(isolate);
        obj->SetInternalFieldCount(1);

        Local<Object> objInstance = obj->NewInstance();
        objInstance->SetInternalField(0, External::New(isolate, responseDataGlobal));
        args[0] = objInstance;

        auto jsCallbackLocal = jsCallback->Get(isolate);
        jsCallbackLocal->Call(jsCallbackLocal, 1, args);
    }));
}

JNIEXPORT void JNICALL Java_com_voicemynews_core_bindings_network_HttpClientBindingParseStringContentAction_invokeJsCallback(
    JNIEnv* env,
    jobject thisObj,
    jstring response)
{
    auto jsCallbackPtr = env->GetLongField(thisObj, HttpClientBindingParseStringContentActionJsCallbackPtr);
    auto isolatePtr = env->GetLongField(thisObj, HttpClientBindingParseStringContentActionIsolatePtr);
    auto isolate = reinterpret_cast<Isolate*>(isolatePtr);
    auto jsCallback = reinterpret_cast<Persistent<Function>*>(jsCallbackPtr);
    auto responseGlobal = (jstring)env->NewGlobalRef(response);
    auto eventLoop = voicemynews::core::events::EventLoopPlatform::GetInstance();

    eventLoop->EnqueueTask(std::function<void()>([&jsCallback, &isolate, &responseGlobal]() {
        JNIEnv* env = nullptr;
        javaVM->AttachCurrentThread(&env, nullptr);

        Local<Value> args[1];
        args[0] = String::NewFromUtf8(isolate, env->GetStringUTFChars(responseGlobal, nullptr));

        auto jsCallbackLocal = jsCallback->Get(isolate);
        jsCallbackLocal->Call(jsCallbackLocal, 1, args);
    }));
}

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
void HttpClientBinding::Get(const std::string& url, const std::map<std::string, std::string>& headers,
              const std::map<std::string, std::string>& queryParams,
              voicemynews::core::network::HttpClientResponseStringCallback handleResponse = nullptr)
{
    // TODO [rcosnita] implement the native reusable http get method.
    throw std::exception();
}

/**
 * \brief Provides the implementation of js insert method belonging to headers map.
 */
static void InsertJsHttpClientBindingHeadersMapValue(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate inputs.
    Isolate* isolate = info.GetIsolate();
    auto holder = info.Holder();

    std::string headerName = *String::Utf8Value(info[0]->ToString());
    std::string headerValue = *String::Utf8Value(info[1]->ToString());

    JNIEnv* env = nullptr;
    javaVM->AttachCurrentThread(&env, nullptr);
    auto headerNameJVM = env->NewStringUTF(headerName.c_str());
    auto headerValueJVM = env->NewStringUTF(headerValue.c_str());
    auto headerMapsJVM = static_cast<jobject>(Local<External>::Cast(holder->GetInternalField(0))->Value());

    env->CallVoidMethod(headerMapsJVM, HttpClientHeadersMapPut, headerNameJVM, headerValueJVM);
}

/**
 * \brief Provides the js wrapper for building a headers map compatible with the java http client binding.
 */
static void GetJsHttpClientBindingNewHeadersMap(const FunctionCallbackInfo<Value>& info)
{
    Isolate* isolate = info.GetIsolate();

    JNIEnv* env = nullptr;
    javaVM->AttachCurrentThread(&env, nullptr);

    auto javaHeaders = env->NewGlobalRef(env->CallObjectMethod(HttpClientObj, HttpClientGetNewHeadersMap));

    Local<ObjectTemplate> jsMapObj = ObjectTemplate::New(isolate);
    jsMapObj->SetInternalFieldCount(1);
    jsMapObj->Set(isolate, "insert", FunctionTemplate::New(isolate, InsertJsHttpClientBindingHeadersMapValue));

    Local<Object> jsMapInstance = jsMapObj->NewInstance();
    jsMapInstance->SetInternalField(0, External::New(isolate, javaHeaders));

    info.GetReturnValue().Set(jsMapInstance);
}

/**
 * \brief Provides the http get js method.
 *
 * Internally, it uses the java HttpClientBinding implementation.
 */
static void GetJsHttpClientBinding(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input parameters.
    Isolate* isolate = info.GetIsolate();

    String::Utf8Value urlUtf8(info[0]->ToString());
    std::string urlStd = *urlUtf8;

    Local<Object> headersMap = info[1]->ToObject();
    Local<Function> callback = Local<Function>::Cast(info[2]);
    auto callbackPersistent = new Persistent<Function>(isolate, callback);

    JNIEnv* env = nullptr;
    javaVM->AttachCurrentThread(&env, nullptr);
    jlong callbackPtr = reinterpret_cast<uintptr_t>(callbackPersistent);
    jlong isolatePtr = reinterpret_cast<uintptr_t>(isolate);
    jobject objGetAction = env->NewObject(HttpClientBindingGetActionClass, HttpClientBindingGetActionConstructor, callbackPtr, isolatePtr);
    jstring url = env->NewStringUTF(urlStd.c_str());
    jobject headers = static_cast<jobject>(Local<External>::Cast(headersMap->GetInternalField(0))->Value());

    env->CallVoidMethod(HttpClientObj, HttpClientGetMethod, url, headers, objGetAction);
}

/**
 * \brief Provides the implementation for parseResponseWithStringContent.
 *
 * Internally, it is a thin wrapper over HttpClientBinding java implementation.
 */
static void ParseJsHttpClientBindingGetResponse(const FunctionCallbackInfo<Value>& info)
{
    // TODO [rcosnita] validate input parameters.
    Isolate* isolate = info.GetIsolate();
    auto holder = info.Holder();

    jobject responseData = static_cast<jobject>(Local<External>::Cast(info[0]->ToObject()->GetInternalField(0))->Value());
    Local<Function> callback = Local<Function>::Cast(info[1]);
    auto callbackPersistent = new Persistent<Function>(isolate, callback);

    auto callbackPtr = reinterpret_cast<uintptr_t>(callbackPersistent);
    auto isolatePtr = reinterpret_cast<uintptr_t>(isolate);

    JNIEnv* env = nullptr;
    javaVM->AttachCurrentThread(&env, nullptr);
    auto objInst = env->NewGlobalRef(env->NewObject(HttpClientBindingParseStringContentActionClass, HttpClientBindingParseStringContentActionConstructor, callbackPtr, isolatePtr));
    env->CallVoidMethod(HttpClientObj, HttpClientParseResponseWithStringContent, objInst);
}

/**
 * \brief Binds HttpNativeClient js wrapper to v8 engine.
 */
static void GetJsHttpClientBindingInstance(const FunctionCallbackInfo<Value>& info)
{
    Isolate* isolate = info.GetIsolate();

    Local<ObjectTemplate> httpClient = ObjectTemplate::New(isolate);
    httpClient->Set(isolate, "get", FunctionTemplate::New(isolate, GetJsHttpClientBinding));
    httpClient->Set(isolate, "getNewHeadersMap", FunctionTemplate::New(isolate, GetJsHttpClientBindingNewHeadersMap));
    httpClient->Set(isolate, "parseResponseWithStringContent", FunctionTemplate::New(isolate, ParseJsHttpClientBindingGetResponse));

    info.GetReturnValue().Set(httpClient->NewInstance());
}

void HttpClientBinding::WireToJs(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> obj)
{
    Local<ObjectTemplate> httpClient = ObjectTemplate::New(isolate);
    obj->Set(isolate, "HttpClient", httpClient);
    httpClient->Set(isolate, "getInstance", FunctionTemplate::New(isolate, GetJsHttpClientBindingInstance));
}

}
}
}
}

namespace voicemynews {
namespace core {
namespace network {
using voicemynews::app::android::bindings::HttpClientBinding;

std::shared_ptr<HttpClientInterface> HttpClientInterface::GetHttpClientInstance()
{
    return std::make_shared<HttpClientBinding>();
}
}
}
}