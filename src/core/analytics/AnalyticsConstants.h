#ifndef VOICEMYNEWSCORE_ANALYTICS_ANALYTICSCONSTANTS_H_
#define VOICEMYNEWSCORE_ANALYTICS_ANALYTICSCONSTANTS_H_

#include <string>

namespace voicemynews {
namespace core {
namespace analytics {
namespace constants {
namespace categories {
    const std::string kAppLifecycle = "app_cycle";
    const std::string kJsGeniusNewsOnScreen = "genius news onscreen";
    const std::string kJsIndividualNewsOnScreen = "individual news onscreen";
}

namespace actions {
    const std::string kAppStartAction = "start-native";
    const std::string kJsStartAction = "start-js";
    const std::string kJsReadAction = "read-js";
    const std::string kJsPauseAction = "pause-js";
    const std::string kJsResumeAction = "resume-js";
    const std::string kJsSkipAction = "skip-js";
}

namespace labels {
    const std::string kAppStartLabel = "Native business logic started";
    const std::string kJsStartLabel = "JS Business Logic Started";
    const std::string kJsReadLabel = "JS Business Logic Read News";
    const std::string kJsPauseLabel = "JS Business Logic Pause News";
    const std::string kJsResumeLabel = "JS Business Logic Resume News";
    const std::string kJsSkipLabel = "JS Business Logic Skip News";
}
}
}
}
}

#endif /* VOICEMYNEWSCORE_ANALYTICS_ANALYTICSCONSTANTS_H_ */