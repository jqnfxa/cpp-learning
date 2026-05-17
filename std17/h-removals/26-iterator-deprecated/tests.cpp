#include <gtest/gtest.h>

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

#include "solution.hpp"

namespace t = task26;

TEST(iterator_deprecated, dereferences_value)
{
    t::counter_iter it(5);
    EXPECT_EQ(*it, 5);
}

TEST(iterator_deprecated, pre_increment)
{
    t::counter_iter it(5);
    ++it;
    EXPECT_EQ(*it, 6);
}

TEST(iterator_deprecated, post_increment_returns_old)
{
    t::counter_iter it(5);
    auto old = it++;
    EXPECT_EQ(*old, 5);
    EXPECT_EQ(*it, 6);
}

TEST(iterator_deprecated, equality)
{
    t::counter_iter a(5);
    t::counter_iter b(5);
    t::counter_iter c(7);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(iterator_deprecated, has_iterator_typedefs)
{
    /* C++17: directly-declared typedefs make the iterator
     * compatible with iterator_traits and STL algorithms. */
    using traits = std::iterator_traits<t::counter_iter>;
    EXPECT_TRUE((std::is_same<traits::value_type, int>::value));
    EXPECT_TRUE(
        (std::is_same<traits::iterator_category,
                      std::forward_iterator_tag>::value));
}

TEST(iterator_deprecated, works_in_std_distance)
{
    t::counter_iter b(0);
    t::counter_iter e(5);
    EXPECT_EQ(std::distance(b, e), 5);
}

TEST(iterator_deprecated, range_for_works)
{
    std::vector<int> seen;
    for (t::counter_iter it(0); it != t::counter_iter(5); ++it)
    {
        seen.push_back(*it);
    }
    EXPECT_EQ(seen, (std::vector<int>{0, 1, 2, 3, 4}));
}
