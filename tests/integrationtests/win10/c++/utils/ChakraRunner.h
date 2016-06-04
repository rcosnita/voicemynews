#ifndef VoiceMyNewsTests_win10_cpp_utils_ChakraRunner_H_
#define VoiceMyNewsTests_win10_cpp_utils_ChakraRunner_H_

#include <jsrt.h>

#include "io/fs/FileUtils.h"

namespace voicemynews {
namespace tests {
namespace app {
namespace win10 {
namespace utils {
namespace js {
using Platform::String;
using voicemynews::core::io::fs::FileUtils;

class ChakraRunner;

/**
 * \brief This callback defines the contract which allows users to receive feedback from running a script in chakra.
 */
typedef void(*JsScriptRunnerCb)(const JsErrorCode& jsErrorCode, const JsValueRef& result, ChakraRunner* runner);

/**
 * \class ChakraRunner
 * \brief This class provides a thin wrapper for bringing up chakra instances and allowing developers to run scripts.
 *
 * The chakra runner class provides a helper for bringing up an embedded chakra instance and executing scripts into it.
 * It can also automatically provides a requirejs implementation which is globally available.
 */
class ChakraRunner {
public:
    ChakraRunner(bool enableRequire);
    virtual ~ChakraRunner();

    /**
     * \brief This method allows developers to runt the given script.
     *
     * \param jsScript the script we want to run in the current chakra instance.
     * \param fileName an optional fileName from where the script was extracted.
     * \param handleResult an optional callback which is used to handle results from the current script execution.
     */
    JsValueRef RunScript(String^ jsScript, String^ fileName = "", JsScriptRunnerCb handleResult = nullptr);

    /**
     * \brief This method provides a thin wrapper over chakra api which allows value conversion to string.
     */
    JsErrorCode ConvertValueToString(JsValueRef value, JsValueRef *result);

    /**
     * \brief This method provides a thin wrapper over chakra api which allows a value to be converted to a wide string.
     */
    JsErrorCode ConvertStringToPointer(JsValueRef value, const wchar_t **str, size_t *stringLength);

    /**
     * \brief This method provides a thin wrapper over chakra api for obtaining the undefined value.
     */
    JsValueRef GetUndefinedValue();
private:
    FileUtils fileUtils_;
    JsRuntimeHandle runtime_;
    JsContextRef context_;
    unsigned int currentSourceContext_;
};
}
}
}
}
}
}

#endif // VoiceMyNewsTests_win10_cpp_utils_ChakraRunner_H_