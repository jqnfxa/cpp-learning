#include <gtest/gtest.h>

#include <map>
#include <string>

#include "solution.hpp"

namespace t = task04;

TEST(if_init_statement, lookup_present)
{
    std::map<int, int> m = {{1, 100}, {2, 200}};
    EXPECT_EQ(t::lookup_or_default(m, 1, -1), 100);
}

TEST(if_init_statement, lookup_present_2)
{
    std::map<int, int> m = {{1, 100}, {2, 200}};
    EXPECT_EQ(t::lookup_or_default(m, 2, -1), 200);
}

TEST(if_init_statement, lookup_missing_returns_default)
{
    std::map<int, int> m = {{1, 100}, {2, 200}};
    EXPECT_EQ(t::lookup_or_default(m, 99, -1), -1);
}

TEST(if_init_statement, lookup_default_zero)
{
    /* Edge case: default is 0, key missing -- matches the
     * stub which returns default. */
    std::map<int, int> m = {{1, 100}};
    EXPECT_EQ(t::lookup_or_default(m, 99, 0), 0);
}

TEST(if_init_statement, lookup_empty_map)
{
    std::map<int, int> m;
    EXPECT_EQ(t::lookup_or_default(m, 1, 42), 42);
}

TEST(if_init_statement, doubled_present)
{
    std::map<int, int> m = {{5, 50}};
    EXPECT_EQ(t::lookup_doubled(m, 5), 100);
}

TEST(if_init_statement, doubled_missing_returns_zero)
{
    /* Matches the stub. Sanity test. */
    std::map<int, int> m;
    EXPECT_EQ(t::lookup_doubled(m, 5), 0);
}

TEST(if_init_statement, doubled_multiple_keys)
{
    std::map<int, int> m = {{1, 10}, {2, 20}, {3, 30}};
    EXPECT_EQ(t::lookup_doubled(m, 1), 20);
    EXPECT_EQ(t::lookup_doubled(m, 2), 40);
    EXPECT_EQ(t::lookup_doubled(m, 3), 60);
}

TEST(if_init_statement, classify_non_positive_zero)
{
    EXPECT_EQ(t::classify(0), "non-positive");
}

TEST(if_init_statement, classify_non_positive_negative)
{
    EXPECT_EQ(t::classify(-5), "non-positive");
}

TEST(if_init_statement, classify_small)
{
    EXPECT_EQ(t::classify(5), "small");
}

TEST(if_init_statement, classify_large)
{
    EXPECT_EQ(t::classify(100), "large");
}

TEST(if_init_statement, classify_boundary_ten)
{
    EXPECT_EQ(t::classify(10), "large");
}

TEST(if_init_statement, classify_boundary_nine)
{
    EXPECT_EQ(t::classify(9), "small");
}
