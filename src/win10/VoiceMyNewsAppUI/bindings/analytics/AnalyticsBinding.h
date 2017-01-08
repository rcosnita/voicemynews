#include "pch.h"

#ifndef VOICEMYNEWSWIN10_BINDINGS_ANALYTICS_ANALYTICSBINDINGS_H_
#define VOICEMYNEWSWIN10_BINDINGS_ANALYTICS_ANALYTICSBINDINGS_H_

#include "analytics/Analytics.h"

namespace voicemynews {
namespace app {
namespace win10 {
namespace bindings {
namespace analytics {
/**
 * \brief A simple js wrapper which can be projected to js side.
 */
public ref class AnalyticsBindingEvent sealed
{
using String = Platform::String;

public:
    AnalyticsBindingEvent(int eventType, String^ eventCategory, String^ eventAction, String^ eventLabel,
        int eventValue);

    int EventType();

    String^ EventCategory();

    String^ EventAction();

    String^ EventLabel();

    int EventValue();

internal:
    operator voicemynews::core::analytics::AnalyticsEvent();

private:
    int eventType_;
    String^ eventCategory_;
    String^ eventAction_;
    String^ eventLabel_;
    int eventValue_;
};

/**
 * \brief Provides the binding class for analytics engine.
 */
public ref class AnalyticsBinding sealed
{
using String = Platform::String;
public:
    /**
     * \brief Obtains the current instance of the analytics engine.
     *
     * In case no instance is ready a new instance will be instantiated.
     */
    static AnalyticsBinding^ GetInstance();

    /**
     * \brief Implements the bootstrap logic for analytics engine.
     */
    static void StartAnalytics();

public:
    /**
     * \brief Obtains a native event instance from the received input parameters.
     */
    AnalyticsBindingEvent^ BuildEvent(int evtType, String^ evtCategory, String^ evtAction, String^ evtLabel,
        int evtValue);

    /**
     * \brief Provides a thin wrapper over analytics native LogEvent implementation.
     *
     * Internally, it just converts the AnalyticsBindingEvent to AnalyticsEvent and pass the newly obtained instance
     * to native LogEvent implementation.
     *
     * Make sure you start logging events from js business logic after analytics layer start event has been received.
     */
    void LogEvent(AnalyticsBindingEvent^ evt);

private:
    AnalyticsBinding();

private:
    std::shared_ptr<voicemynews::core::analytics::Analytics> analytics_;
};
}
}
}
}
}

#endif /* VOICEMYNEWSWIN10_BINDINGS_ANALYTICS_ANALYTICSBINDINGS_H_ */