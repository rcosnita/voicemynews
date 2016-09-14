#include "pch.h"
#include "EventLoopBinding.h"
#include "events/EventData.h"
#include "utils/Conversions.h"

using voicemynews::app::win10::utils::ConvertPlatformStrToStd;
using voicemynews::app::win10::utils::ConvertStdStrToPlatform;
using voicemynews::core::events::EventData;
using voicemynews::core::events::EventLoopPlatform;


namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace events {

static EventLoopBinding^ loopInstance = nullptr;

/**
 * \brief This method constructs a unique string identifier for the given event handler pointer.
 */
static std::string GetStrForEventHandlerPtr(String^ evtName, EventHandler^ evtHandler)
{
    auto key = evtName + ":" + evtHandler->GetType()->FullName + ":" + evtHandler->GetHashCode();

    return ConvertPlatformStrToStd(key);
}

EventLoopBinding::EventLoopBinding()
{
}

EventLoopBinding::EventLoopBinding(bool processImmediate)
    : processImmediate_(processImmediate)
{
}

EventLoopBinding^ EventLoopBinding::GetInstance()
{
    if (loopInstance == nullptr) {
        loopInstance = ref new EventLoopBinding();
    }

    return loopInstance;
}

void EventLoopBinding::Emit(String^ evtName, EventDataBinding^ evtData)
{
    eventLoop_.Emit(ConvertPlatformStrToStd(evtName),
        std::make_shared<EventLoopBinding::EventData>(ConvertPlatformStrToStd(evtData->EvtData)));

    if (processImmediate_) {
        ProcessEvents();
    }
}

void EventLoopBinding::EnqueueTask(JsLoopEnqueuedTask^ task)
{
    deferredTasks_.push(task);
}

String^ EventLoopBinding::On(String^ evtName, EventHandler^ handler)
{
    std::unique_lock<std::mutex> listenersLock;
    auto handlerKey = GetStrForEventHandlerPtr(evtName, handler);

    auto evtNameStd = ConvertPlatformStrToStd(evtName);
    auto evtHandlerPtr = eventLoop_.On(evtNameStd, std::function<void(std::shared_ptr<EventLoopBinding::EventData>)>([handler](std::shared_ptr<EventLoopBinding::EventData> evtDataStd) {
        auto evtData = EventLoopPlatform::BuildEvent(ConvertStdStrToPlatform(evtDataStd->data()));
        handler(evtData);
    }));

    registeredListeners_[handlerKey] = ListenerModel(evtName, evtHandlerPtr);

    return ConvertStdStrToPlatform(handlerKey);
}

void EventLoopBinding::Off(String^ handlerId)
{
    auto handlerKey = ConvertPlatformStrToStd(handlerId);
    std::unique_lock<std::mutex> listenersLock;
    auto entry = registeredListeners_.find(handlerKey);
    auto listenerModel = entry->second;
    auto evtNameStd = ConvertPlatformStrToStd(listenerModel.evtName);
    eventLoop_.Off(evtNameStd, listenerModel.handlerPtr);
    registeredListeners_.erase(entry);
}

void EventLoopBinding::ProcessEvents()
{
    eventLoop_.ProcessEvents();

    while (!deferredTasks_.empty()) {
        auto task = deferredTasks_.back();
        task();
        deferredTasks_.pop();
    }
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