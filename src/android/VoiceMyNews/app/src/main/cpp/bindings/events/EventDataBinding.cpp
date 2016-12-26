#include "bindings/events/EventDataBinding.h"
#include "EventDataBinding.h"

namespace voicemynews {
namespace app {
namespace android {
namespace bindings {
namespace events {
EventDataBinding::EventDataBinding(std::string data) : voicemynews::core::events::EventData<std::string>(data)
{
}
}
}
}
}
}