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
 * \brief Provides the binding class for analytics engine.
 */
public ref class AnalyticsBinding sealed {
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

private:
    AnalyticsBinding();

private:
    std::shared_ptr<voicemynews::core::analytics::Analytics> analytics;
};
}
}
}
}
}

#endif /* VOICEMYNEWSWIN10_BINDINGS_ANALYTICS_ANALYTICSBINDINGS_H_ */