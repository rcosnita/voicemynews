#ifdef DEV_BUILD
#include "Config.h"

namespace voicemynews {
namespace core {
namespace config {
    const std::string kWebBaseUrl = "http://readmy.news/voicemynews/analytics-tracking-dev.html";

    const std::string kAnalyticsGaTrackingCode = "<script>\n"
        "  (function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){\n"
        "  (i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),\n"
        "  m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)\n"
        "  })(window,document,'script','https://www.google-analytics.com/analytics_debug.js','ga');\n"
        "\n"
        "  window.ga_debug = {trace: true};\n"
        "  ga('create', 'UA-84360208-5', 'auto');\n"
        "  ga('set', 'transport', 'beacon');\n"
        "  ga('set', 'appName', 'VoiceMyNewsDev');\n"
        "  ga('set', 'appVersion', '0.0.1');\n"
        "  console.log('ga initialized ...');\n"
        "\n"
        "</script>";
}
}
}
#endif