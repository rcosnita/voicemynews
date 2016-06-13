#include "pch.h"
#include "CommonAssertions.h"

#include "CppUnitTest.h"

namespace voicemynews {
namespace tests {
namespace app {
namespace win10 {
namespace utils {
namespace assertions {
using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

    void AssertNoJsError(const JsErrorCode jsErrorCode) {
        Assert::AreEqual(static_cast<int>(JsErrorCode::JsNoError), static_cast<int>(jsErrorCode));
    }
}
}
}
}
}
}