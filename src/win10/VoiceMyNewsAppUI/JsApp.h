#ifndef VoiceMyNewsAppUI_JsApp_H_
#define VoiceMyNewsAppUI_JsApp_H_

#include <jsrt.h>

#include "io/fs/FileUtils.h"
#include "bindings/events/EventLoopBinding.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace js {
using voicemynews::core::io::fs::FileUtils;
using voicemynews::app::win10::bindings::events::EventLoopBinding;

/**
 * \brief This class provides the mechanism for launching voicemynews js business logic.
 * Internally, it starts a chakra engine and project all required namespaces to it.
 */
public ref class JsApp sealed {
public:
    JsApp(EventLoopBinding^ eventLoop);
    virtual ~JsApp();

    /**
     * \brief This method returns the current instance of the js application backend.
     */
    static JsApp^ GetInstance();

    /**
     * \brief Start the js business logic of voicemynews application.
     */
    void Start();

    /**
     * \brief Obtains the current event loop used by the js business logic.
     */
    EventLoopBinding^ GetEventLoop();

private:
    EventLoopBinding^ eventLoop_;
    FileUtils fileUtils_;
    JsRuntimeHandle runtime_;
    JsContextRef context_;
    JsValueRef promiseCallback_;
    unsigned int currentSourceContext_;

private:
    /**
     * \brief This method bind a require implementation to the global js context so that we can use CommonJS.
     */
    JsErrorCode BindRequireJs();

    /**
     * \brief This method loads the app.js main entry point into the currently instantiated chakra module.
     * From this moment on the application can communicate with the js logic through events. The same message
     * bus can also be used to receive events from js.
     */
    JsErrorCode StartApp();
};
}
}
}
}

#endif /* VoiceMyNewsAppUI_JsApp_H_ */