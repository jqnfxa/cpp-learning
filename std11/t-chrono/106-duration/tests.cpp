#include <gtest/gtest.h>

#include <chrono>

#include "solution.hpp"

TEST(chrono_duration, five_seconds_value)
{
    EXPECT_EQ(task106::five_seconds(), std::chrono::seconds(5));
    EXPECT_EQ(task106::five_seconds().count(), 5);
}

TEST(chrono_duration, hundred_milliseconds_value)
{
    EXPECT_EQ(task106::hundred_milliseconds(), std::chrono::milliseconds(100));
    EXPECT_EQ(task106::hundred_milliseconds().count(), 100);
}

TEST(chrono_duration, count_of_seconds)
{
    EXPECT_EQ(task106::count_of(std::chrono::seconds(42)), 42);
    EXPECT_EQ(task106::count_of(std::chrono::seconds(0)), 0);
    EXPECT_EQ(task106::count_of(std::chrono::seconds(-1)), -1);
}

TEST(chrono_duration, add_seconds_and_milliseconds)
{
    auto sum = task106::add(std::chrono::seconds(2),
                            std::chrono::milliseconds(500));
    EXPECT_EQ(sum, std::chrono::milliseconds(2500));
    EXPECT_EQ(sum.count(), 2500);
}

TEST(chrono_duration, add_zero_milliseconds)
{
    auto sum = task106::add(std::chrono::seconds(7),
                            std::chrono::milliseconds(0));
    EXPECT_EQ(sum, std::chrono::milliseconds(7000));
}

TEST(chrono_duration, add_only_milliseconds)
{
    auto sum = task106::add(std::chrono::seconds(0),
                            std::chrono::milliseconds(250));
    EXPECT_EQ(sum, std::chrono::milliseconds(250));
}

TEST(chrono_duration, implicit_no_loss_conversion)
{
    /* seconds(2) -> milliseconds(2000) implicitly. */
    std::chrono::milliseconds ms = std::chrono::seconds(2);
    EXPECT_EQ(ms.count(), 2000);
}

TEST(chrono_duration, type_pins)
{
    static_assert(std::is_same<decltype(task106::five_seconds()),
                               std::chrono::seconds>::value,
                  "five_seconds returns seconds");
    static_assert(std::is_same<decltype(task106::hundred_milliseconds()),
                               std::chrono::milliseconds>::value,
                  "hundred_milliseconds returns milliseconds");
    SUCCEED();
}
