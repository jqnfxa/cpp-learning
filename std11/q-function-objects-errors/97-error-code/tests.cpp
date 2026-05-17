#include <gtest/gtest.h>

#include <cerrno>
#include <cstdio>
#include <string>
#include <system_error>

#include "solution.hpp"

namespace
{

class error_code_test : public ::testing::Test
{
protected:
    std::string existing_path = ::testing::TempDir() + "/task97_existing.txt";

    void SetUp() override
    {
        /* Create a small file we can successfully open. */
        FILE *f = std::fopen(existing_path.c_str(), "w");
        ASSERT_NE(f, nullptr) << "test setup: failed to create " << existing_path;
        std::fputs("hi", f);
        std::fclose(f);
    }

    void TearDown() override
    {
        std::remove(existing_path.c_str());
    }
};

}

TEST_F(error_code_test, open_existing_succeeds)
{
    FILE *f = task97::open_for_read(existing_path);
    ASSERT_NE(f, nullptr);
    task97::close_file(f);
}

TEST_F(error_code_test, open_missing_throws_system_error)
{
    EXPECT_THROW(task97::open_for_read("/this/path/should/not/exist/task97"),
                 std::system_error);
}

TEST_F(error_code_test, open_missing_error_code_is_ENOENT)
{
    try
    {
        task97::open_for_read("/this/path/should/not/exist/task97");
        FAIL() << "expected system_error";
    }
    catch (const std::system_error &e)
    {
        EXPECT_EQ(e.code().value(), ENOENT);
    }
}

TEST_F(error_code_test, open_missing_category_is_generic)
{
    try
    {
        task97::open_for_read("/this/path/should/not/exist/task97");
        FAIL() << "expected system_error";
    }
    catch (const std::system_error &e)
    {
        EXPECT_EQ(&e.code().category(), &std::generic_category());
    }
}

TEST_F(error_code_test, open_missing_message_has_prefix)
{
    const std::string path = "/this/path/should/not/exist/task97";
    try
    {
        task97::open_for_read(path);
        FAIL();
    }
    catch (const std::system_error &e)
    {
        std::string msg = e.what();
        EXPECT_NE(msg.find("open failed: " + path), std::string::npos)
            << "message was: " << msg;
    }
}

TEST_F(error_code_test, error_code_compares_to_errc_portably)
{
    /* The portable / cross-platform check: errc::no_such_file_or_directory
     * maps to ENOENT on POSIX. */
    try
    {
        task97::open_for_read("/this/path/should/not/exist/task97");
        FAIL();
    }
    catch (const std::system_error &e)
    {
        EXPECT_TRUE(e.code() == std::errc::no_such_file_or_directory);
    }
}

TEST_F(error_code_test, close_file_handles_null)
{
    /* close_file on null must not crash and must not throw. */
    task97::close_file(nullptr);
    SUCCEED();
}
