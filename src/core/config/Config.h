#ifndef VOICEMYNEWSCORE_CONFIG_CONFIG_H_
#define VOICEMYNEWSCORE_CONFIG_CONFIG_H_

#include <string>

namespace voicemynews {
namespace core {
namespace config {
    /**
     * \brief Provides the base url which must be used by all app web views.
     */
    extern const std::string kWebBaseUrl;

    /**
     * \brief Provides the tracking code for google analytics.
     *
     * We are using various tracking codes for various environments.
     */
    extern const std::string kAnalyticsGaTrackingCode;
}
}
}

#endif /* VOICEMYNEWSCORE_CONFIG_CONFIG_H_ */