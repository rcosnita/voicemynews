#include "pch.h"
#include "EventLoopBinding.h"
#include "events/EventData.h"
#include "utils/Conversions.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {
using voicemynews::app::win10::utils::ConvertPlatformStrToStd;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using voicemynews::core::events::EventData;
using voicemynews::core::events::EventLoopPlatform;

static EventLoopBinding^ loopInstance = nullptr;

EventLoopBinding::EventLoopBinding() {
}

EventLoopBinding^ EventLoopBinding::GetInstance() {
    if (loopInstance == nullptr) {
        loopInstance = ref new EventLoopBinding();
    }

    return loopInstance;
}

void EventLoopBinding::Emit(String^ evtName, EventDataBinding^ evtData) {
    eventLoop_.Emit(ConvertPlatformStrToStd(evtName),
        std::make_shared<EventData<std::string>>(ConvertPlatformStrToStd(evtData->EvtData)));
}

void EventLoopBinding::On(String^ evtName, EventHandler^ handler) {
    auto evtNameStd = ConvertPlatformStrToStd(evtName);
    std::function<void(std::shared_ptr<EventData<std::string>>)> fn = [handler](std::shared_ptr<EventData<std::string>> evtDataStd) {
        auto evtData = EventLoopPlatform::BuildEvent(ConvertStdStrToPlatform(evtDataStd->data()));
        handler(evtData);
    };

    eventLoop_.On(evtNameStd, fn);
}

void EventLoopBinding::ProcessEvents() {
    eventLoop_.ProcessEvents();
}
}
}
}
}
}

namespace voicemynews {
namespace core {
namespace events {
EventLoopBinding^ EventLoopPlatform::GetInstance() {
    return EventLoopBinding::GetInstance();
}

EventDataBinding^ EventLoopPlatform::BuildEvent(String^ data) {
    return ref new EventDataBinding(data);
}
}
}
}