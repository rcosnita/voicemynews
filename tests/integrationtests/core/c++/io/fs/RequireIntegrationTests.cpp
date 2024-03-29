#include <memory>
#include <string>

#include "io/fs/Require.h"
#include "io/fs/exceptions/FileNotFoundException.h"
#include "gtest/gtest.h"
#include "utils/TestIOUtils.h"

namespace voicemynews {
namespace tests {
namespace integration {
namespace core {
namespace io {
namespace fs {
using voicemynews::core::io::fs::FileUtils;
using voicemynews::core::io::fs::Require;
using voicemynews::core::io::fs::exceptions::FileNotFoundException;
using voicemynews::tests::integration::utils::TestIOUtils;

class RequireIntegrationTests : public ::testing::Test{
public:
    RequireIntegrationTests() {}
    virtual ~RequireIntegrationTests() = default;

    virtual void SetUp() { 
        require_ = std::make_unique<Require>(std::make_shared<FileUtils>());
    }
    virtual void TearDown() { }

public:
    std::unique_ptr<TestIOUtils> ioUtils_;
    std::unique_ptr<Require> require_;
};

/**
 * \brief This test case ensures the files are correctly decorated with anonymous requirejs function.
 */
TEST_F(RequireIntegrationTests, FileCorrectlyResolved) {
    std::string fileName = "samples/integrations/io/fs/app.js";
    std::string expectedFile = "samples/integrations/io/fs/app_expected.js";

    auto result = require_->Load(fileName);
    auto expectedResult = ioUtils_->ReadFile(expectedFile);

    EXPECT_EQ(expectedResult, result);
}

/**
 * \brief This test case ensures the files are correctly decorated with anonymous requirejs function even though the
 * files content is not valid js code..
 */
TEST_F(RequireIntegrationTests, CorruptedJsCorrectlyResolved) {
    std::string fileName = "samples/integrations/io/fs/app_corrupted.js";
    std::string expectedFile = "samples/integrations/io/fs/app_corrupted_expected.js";

    auto result = require_->Load(fileName);
    auto expectedResult = ioUtils_->ReadFile(expectedFile);

    EXPECT_EQ(expectedResult, result);
}

/**
 * \brieft This test case ensures concrete exceptions are raised when trying to load a module which does not exist.
 */
TEST_F(RequireIntegrationTests, FileNotFoundException) {
    std::string fileName = "samples/integrations/io/fs/unknown_file.xxnonoon";

    try {
        require_->Load(fileName);
        FAIL() << "Expected FileNotFound exception for file " << fileName;
    }
    catch (const FileNotFoundException& ex) {
        EXPECT_EQ(fileName, ex.FileName());
    }
    catch (...) {
        FAIL() << "Expected FileNotFound exception for file " << fileName;
    }
}

/**
 * \brief This test case ensures load raw correctly loads a js file.
 */
TEST_F(RequireIntegrationTests, LoadRawJsOk) {
    std::string fileName = "samples/integrations/io/fs/app.js";

    std::wstring loadedContent = require_->LoadRaw(fileName);
    std::wstring expectedContent = ioUtils_->ReadFile(fileName);

    EXPECT_EQ(expectedContent, loadedContent);
}

/**
 * \brief This test case ensures load raw correctly loads a text file.
 */
TEST_F(RequireIntegrationTests, LoadRawTextOk) {
    std::string fileName = "samples/integrations/io/fs/raw_sample.txt";

    std::wstring loadedContent = require_->LoadRaw(fileName);
    std::wstring expectedContent = ioUtils_->ReadFile(fileName);

    EXPECT_EQ(expectedContent, loadedContent);
}

/**
 * \brief This test case ensures load raw raises a concrete exception in case an attempt to load a missing file is done.
 */
TEST_F(RequireIntegrationTests, LoadRawFileNotFound) {
    std::string fileName = "samples/integrations/io/fs/unknown_file.xxnonoon";

    try {
        require_->LoadRaw(fileName);
        FAIL() << "Expected FileNotFound exception for file " << fileName;
    } catch (const FileNotFoundException& ex) {
        EXPECT_EQ(fileName, ex.FileName());
    } catch (...) {
        FAIL() << "Expected FileNotFound exception for file " << fileName;
    }
}

}
}
}
}
}
}