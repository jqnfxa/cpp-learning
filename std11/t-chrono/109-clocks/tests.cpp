#include <gtest/gtest.h>

#include <cstring>

#include "solution.hpp"

TEST(clocks, timeout_uses_steady)
{
    /* A timeout wants a deadline that survives wall-clock jumps.
     * steady_clock is the right choice. */
    EXPECT_STREQ(task109::clock_for_timeout, "steady_clock");
}

TEST(clocks, profiling_uses_steady)
{
    /* Measuring how long something took -- monotonic clock. */
    EXPECT_STREQ(task109::clock_for_profiling, "steady_clock");
}

TEST(clocks, wall_time_uses_system)
{
    /* Human-readable timestamp -- wall clock. */
    EXPECT_STREQ(task109::clock_for_wall_time, "system_clock");
}

TEST(clocks, none_use_high_resolution)
{
    /* high_resolution_clock is portably ambiguous (aliased to
     * system_clock on libstdc++, steady_clock elsewhere). Don't
     * use it for any of these scenarios. */
    EXPECT_STRNE(task109::clock_for_timeout, "high_resolution_clock");
    EXPECT_STRNE(task109::clock_for_profiling, "high_resolution_clock");
    EXPECT_STRNE(task109::clock_for_wall_time, "high_resolution_clock");
}
