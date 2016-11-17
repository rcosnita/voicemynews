#ifndef VoiceMyNewsAndroid_bindings_RequireBinding_H_
#define VoiceMyNewsAndroid_bindings_RequireBinding_H_

#include "v8.h"

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace require {
    /**
     * \brief Provides a thin wrapper over load method from require.
     *
     * It can be used from javascript.
     */
    void LoadRequire(const v8::FunctionCallbackInfo<v8::Value>& info);

    /**
     * \brief Provides a thin wrapper over loadRaw method from require.
     *
     * It can be used from javascript.
     */
    void LoadRawRequire(const v8::FunctionCallbackInfo<v8::Value>& info);

    /**
     * \brief Provides the algorithm for obtaining a new instance of require native object.
     */
    void GetRequireInstance(const v8::FunctionCallbackInfo<v8::Value>& info);
}
}
}
}
}

#endif /* VoiceMyNewsAndroid_bindings_RequireBinding_H_ */
