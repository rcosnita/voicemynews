#include "pch.h"
#include "EventDataBinding.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
using Platform::String;

EventDataBinding::EventDataBinding(String^ data)
    : data_(data) {
}

String^ EventDataBinding::EvtData::get() {
    return data_;
}
}
}
}
}
}