#include "pch.h"
#include "EventLoopBinding.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
static EventLoopBinding^ loopInstance = nullptr;

EventLoopBinding^ EventLoopBinding::GetInstance() {
    if (loopInstance == nullptr) {
        loopInstance = ref new EventLoopBinding();
    }

    return loopInstance;
}

void EventLoopBinding::Emit(String^ evtName, EventDataBinding^ evtData) {
    throw ref new Platform::Exception(-1000, "Not implemented yet ...");
}

void EventLoopBinding::On(String^ evtName, EventHandler^ handler) {
    throw ref new Platform::Exception(-1000, "Not implemented yet ...");
}

void EventLoopBinding::ProcessEvents() {
    throw ref new Platform::Exception(-1000, "Not implemented yet ...");
}
}
}
}
}
}