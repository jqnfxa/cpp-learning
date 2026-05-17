#include <gtest/gtest.h>

#include <chrono>

#include "solution.hpp"

TEST(duration_cast, to_seconds_truncates_to_lower)
{
    EXPECT_EQ(task107::to_seconds(std::chrono::milliseconds(1500)),
              std::chrono::seconds(1));
    EXPECT_EQ(task107::to_seconds(std::chrono::milliseconds(999)),
              std::chrono::seconds(0));
    EXPECT_EQ(task107::to_seconds(std::chrono::milliseconds(2500)),
              std::chrono::seconds(2));
}

TEST(duration_cast, to_seconds_exact_seconds_preserved)
{
    EXPECT_EQ(task107::to_seconds(std::chrono::milliseconds(0)),
              std::chrono::seconds(0));
    EXPECT_EQ(task107::to_seconds(std::chrono::milliseconds(1000)),
              std::chrono::seconds(1));
    EXPECT_EQ(task107::to_seconds(std::chrono::milliseconds(60000)),
              std::chrono::seconds(60));
}

TEST(duration_cast, to_seconds_truncates_toward_zero_for_negatives)
{
    EXPECT_EQ(task107::to_seconds(std::chrono::milliseconds(-1500)),
              std::chrono::seconds(-1));
    EXPECT_EQ(task107::to_seconds(std::chrono::milliseconds(-999)),
              std::chrono::seconds(0));
}

TEST(duration_cast, to_milliseconds_lossless_multiplication)
{
    EXPECT_EQ(task107::to_milliseconds(std::chrono::seconds(2)),
              std::chrono::milliseconds(2000));
    EXPECT_EQ(task107::to_milliseconds(std::chrono::seconds(0)),
              std::chrono::milliseconds(0));
    EXPECT_EQ(task107::to_milliseconds(std::chrono::seconds(60)),
              std::chrono::milliseconds(60000));
}

TEST(duration_cast, divide_basic)
{
    EXPECT_EQ(task107::divide(std::chrono::milliseconds(1000),
                              std::chrono::milliseconds(250)),
              4);
    EXPECT_EQ(task107::divide(std::chrono::milliseconds(1000),
                              std::chrono::milliseconds(1)),
              1000);
}

TEST(duration_cast, divide_truncates_remainder)
{
    EXPECT_EQ(task107::divide(std::chrono::milliseconds(1000),
                              std::chrono::milliseconds(300)),
              3); /* 1000/300 == 3, remainder 100 dropped */
}

TEST(duration_cast, divide_zero_total)
{
    EXPECT_EQ(task107::divide(std::chrono::milliseconds(0),
                              std::chrono::milliseconds(250)),
              0);
}

TEST(duration_cast, roundtrip_lose_subsecond)
{
    /* Conversion is not invertible when fractional seconds exist. */
    auto original = std::chrono::milliseconds(1750);
    auto secs = task107::to_seconds(original);
    auto roundtrip = task107::to_milliseconds(secs);
    EXPECT_EQ(roundtrip, std::chrono::milliseconds(1000));
}
