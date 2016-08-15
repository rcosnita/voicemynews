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
    eventLoop_.On(evtNameStd,
        std::function<void(std::shared_ptr<EventData<std::string>>)>([handler](std::shared_ptr<EventData<std::string>> evtDataStd) {
        auto evtData = EventLoopPlatform::BuildEvent(ConvertStdStrToPlatform(evtDataStd->data()));
        OutputDebugStringW(L"Invoking handler after data was built...");
        handler(evtData);
    }));
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