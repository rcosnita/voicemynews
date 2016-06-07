#include "pch.h"

#include <ppltasks.h>

#include "bindings/network/HttpClientBinding.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
using Windows::Foundation::Uri;
using Windows::Web::Http::HttpClient;

IAsyncOperationWithProgress<HttpResponseMessage^, HttpProgress>^ HttpClientBinding::Get(Platform::String^ uri) {
    auto httpClient = ref new HttpClient();
    return httpClient->GetAsync(ref new Uri(uri));
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