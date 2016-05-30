#ifndef VoiceMyNewsAppUI_bindings_RequireBinding_H_
#define VoiceMyNewsAppUI_bindings_RequireBinding_H_

#include <string>

#include "io/fs/Require.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
using Platform::String;

using voicemynews::core::io::fs::FileUtils;
using voicemynews::core::io::fs::Require;
/**
 * \class RequireBinding
 * \brief Runtime component used to project native requirejs dependencies into Chakra.
 */
public ref class RequireBinding sealed {
public:
    RequireBinding();

    /**
     * \brief This method returns the module source code from the specified filename.
     *
     * \param fileName the absolute / relative path of the module we want to load.
     * \return the module source code which can be executed by requirejs.
     */
    String^ Load(String^ fileName);
private:
    Require require_;
};
}
}
}
}

#endif // VoiceMyNewsAppUI_bindings_RequireBinding_H_
