#ifndef VOICEMYNEWSCORE_ANALYTICS_ANALYTICSCONSTANTS_H_
#define VOICEMYNEWSCORE_ANALYTICS_ANALYTICSCONSTANTS_H_

#include <string>

namespace voicemynews {
namespace core {
namespace analytics {
namespace constants {
namespace categories {
    const std::string kAppLifecycle = "app_cycle";
}

namespace actions {
    const std::string kAppStartAction = "start-native";
    const std::string kJsStartAction = "start-js";
}

namespace labels {
    const std::string kAppStartLabel = "Native business logic started";
    const std::string kJsStartLabel = "JS Business Logic Started";
}
}
}
}
}

#endif /* VOICEMYNEWSCORE_ANALYTICS_ANALYTICSCONSTANTS_H_ */