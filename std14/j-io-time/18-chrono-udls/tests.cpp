#include <gtest/gtest.h>

#include <chrono>
#include <type_traits>

#include "solution.hpp"

namespace t = task18;
using namespace std::chrono_literals;

TEST(chrono_udls, fifty_ms_equals_50ms)
{
    EXPECT_EQ(t::fifty_ms, std::chrono::milliseconds(50));
}

TEST(chrono_udls, fifty_ms_in_microseconds)
{
    EXPECT_EQ(std::chrono::duration_cast<std::chrono::microseconds>(t::fifty_ms),
              std::chrono::microseconds(50'000));
}

TEST(chrono_udls, two_seconds_equals_2000ms)
{
    EXPECT_EQ(std::chrono::duration_cast<std::chrono::milliseconds>(t::two_seconds),
              std::chrono::milliseconds(2000));
}

TEST(chrono_udls, two_seconds_equals_2s)
{
    EXPECT_EQ(t::two_seconds, std::chrono::seconds(2));
}

TEST(chrono_udls, one_minute_equals_60s)
{
    EXPECT_EQ(std::chrono::duration_cast<std::chrono::seconds>(t::one_minute),
              std::chrono::seconds(60));
}

TEST(chrono_udls, one_day_equals_24h)
{
    EXPECT_EQ(t::one_day, std::chrono::hours(24));
}

TEST(chrono_udls, one_day_in_minutes)
{
    EXPECT_EQ(std::chrono::duration_cast<std::chrono::minutes>(t::one_day),
              std::chrono::minutes(1440));
}

TEST(chrono_udls, five_hundred_ns_equals_500ns)
{
    EXPECT_EQ(t::five_hundred_ns, std::chrono::nanoseconds(500));
}

TEST(chrono_udls, hundred_ms_value)
{
    EXPECT_EQ(t::hundred_ms(), std::chrono::milliseconds(100));
}

TEST(chrono_udls, hundred_ms_type_is_milliseconds)
{
    using R = decltype(t::hundred_ms());
    EXPECT_TRUE((std::is_same<R, std::chrono::milliseconds>::value));
}

TEST(chrono_udls, fifty_ms_type_is_milliseconds)
{
    /* The UDL `50ms` deduces to std::chrono::milliseconds.
     * The stub's `50us` deduces to std::chrono::microseconds. */
    using R = decltype(t::fifty_ms);
    EXPECT_TRUE((std::is_same<R, const std::chrono::milliseconds>::value));
}

TEST(chrono_udls, evaluable_at_compile_time)
{
    /* All the constants are constexpr; usable in constant
     * expressions. */
    constexpr auto d = t::two_seconds;
    EXPECT_EQ(d, 2s);
}
