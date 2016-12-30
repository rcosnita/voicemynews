#ifndef VOICEMYNEWSCORE_CONFIG_CONFIG_H_
#define VOICEMYNEWSCORE_CONFIG_CONFIG_H_

#include <string>

namespace voicemynews {
namespace core {
namespace config {
    /**
     * \brief Provides the base url which must be used by all app web views.
     *
     * For most platform which supports web views and provide loadDataWithUrl this will never be fetched.
     */
    extern const std::string kWebBaseUrl;

    /**
     * \brief Provides the tracking code for google analytics.
     *
     * We are using various tracking codes for various environments. For most operating systems it works. For those
     * operating systems which can load data with base url (e.g windows 10) we must navigate directly to kWebBaseUrl.
     */
    extern const std::string kAnalyticsGaTrackingCode;
}
}
}

#endif /* VOICEMYNEWSCORE_CONFIG_CONFIG_H_ */