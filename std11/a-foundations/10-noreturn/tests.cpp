#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "solution.hpp"

TEST(noreturn_attr, positive_returns_same_value)
{
    EXPECT_EQ(task10::positive_only(5), 5);
    EXPECT_EQ(task10::positive_only(42), 42);
}

TEST(noreturn_attr, zero_returns_zero)
{
    EXPECT_EQ(task10::positive_only(0), 0);
}

TEST(noreturn_attr, negative_throws_runtime_error)
{
    EXPECT_THROW(task10::positive_only(-1), std::runtime_error);
    EXPECT_THROW(task10::positive_only(-100), std::runtime_error);
}

TEST(noreturn_attr, panic_throws_runtime_error)
{
    EXPECT_THROW(task10::panic("test"), std::runtime_error);
}

TEST(noreturn_attr, panic_message_is_propagated)
{
    try
    {
        task10::panic("hello world");
        FAIL() << "panic should have thrown";
    }
    catch (const std::runtime_error &e)
    {
        EXPECT_STREQ(e.what(), "hello world");
    }
}
