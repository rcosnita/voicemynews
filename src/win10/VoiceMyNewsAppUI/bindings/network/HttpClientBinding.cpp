#include "pch.h"

#include <ppltasks.h>

#include "bindings/network/HttpClientBinding.h"
#include "utils/Conversions.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
using Platform::String;

using voicemynews::app::win10::utils::ConvertPlatformMapToStd;
using Windows::Web::Http::HttpClient;

IAsyncOperationWithProgress<HttpResponseMessage^, HttpProgress>^ HttpClientBinding::Get(Uri^ uri) {
    auto httpClient = ref new HttpClient();
    return httpClient->GetAsync(uri);
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace network {
HttpClientPlatform^ HttpClient::GetInstance() {
    return ref new HttpClientPlatform();
}
}
}
}