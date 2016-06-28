#ifndef VoiceMyNewsAppUI_JsApp_H_
#define VoiceMyNewsAppUI_JsApp_H_

#include <jsrt.h>

#include "io/fs/FileUtils.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace js {
using voicemynews::core::io::fs::FileUtils;

/**
 * \brief This class provides the mechanism for launching voicemynews js business logic.
 * Internally, it starts a chakra engine and project all required namespaces to it.
 */
public ref class JsApp sealed {
public:
    JsApp();
    virtual ~JsApp();

    /**
     * \brief Start the js business logic of voicemynews application.
     */
    void Start();

private:
    FileUtils fileUtils_;
    JsRuntimeHandle runtime_;
    JsContextRef context_;
    JsValueRef promiseCallback_;
    unsigned int currentSourceContext_;

private:
    /**
     * \brief This method bind a require implementation to the global js context so that we can use CommonJS.
     */
    void BindRequireJs();

    /**
     * \brief This method loads the app.js main entry point into the currently instantiated chakra module.
     * From this moment on the application can communicate with the js logic through events. The same message
     * bus can also be used to receive events from js.
     */
    void StartApp();
};
}
}
}
}

#endif /* VoiceMyNewsAppUI_JsApp_H_ */