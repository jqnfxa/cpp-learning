#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "solution.hpp"

TEST(range_for_variants, double_in_place_mutates)
{
    std::vector<int> v = {1, 2, 3, 4};
    task17::double_in_place(v);
    EXPECT_EQ(v, (std::vector<int>{2, 4, 6, 8}));
}

TEST(range_for_variants, double_in_place_on_empty)
{
    std::vector<int> v;
    task17::double_in_place(v);
    EXPECT_TRUE(v.empty());
}

TEST(range_for_variants, double_in_place_idempotent_on_zeros)
{
    std::vector<int> v = {0, 0, 0};
    task17::double_in_place(v);
    EXPECT_EQ(v, (std::vector<int>{0, 0, 0}));
}

TEST(range_for_variants, try_double_via_copy_returns_doubled_sum)
{
    std::vector<int> v = {1, 2, 3, 4};
    int doubled_sum = task17::try_double_via_copy(v);
    /* 1*2 + 2*2 + 3*2 + 4*2 = 20 */
    EXPECT_EQ(doubled_sum, 20);
}

TEST(range_for_variants, try_double_via_copy_leaves_v_untouched)
{
    /*
     * The whole point: even though the function "doubles" elements,
     * those mutations happen on per-iteration copies. The caller's
     * vector must remain {1, 2, 3, 4}.
     */
    std::vector<int> v = {1, 2, 3, 4};
    (void)task17::try_double_via_copy(v);
    EXPECT_EQ(v, (std::vector<int>{1, 2, 3, 4}));
}

TEST(range_for_variants, try_double_via_copy_empty)
{
    std::vector<int> v;
    EXPECT_EQ(task17::try_double_via_copy(v), 0);
    EXPECT_TRUE(v.empty());
}

TEST(range_for_variants, count_long_strings)
{
    std::vector<std::string> v = {"a", "bb", "ccc", "dddd"};
    EXPECT_EQ(task17::count_long_strings(v, 3), 2u);
    EXPECT_EQ(task17::count_long_strings(v, 1), 4u);
    EXPECT_EQ(task17::count_long_strings(v, 5), 0u);
}

TEST(range_for_variants, count_long_strings_empty)
{
    std::vector<std::string> v;
    EXPECT_EQ(task17::count_long_strings(v, 3), 0u);
}

TEST(range_for_variants, count_long_strings_threshold_zero)
{
    /* Every string has size >= 0, so the count equals the vector size. */
    std::vector<std::string> v = {"", "x", "abc"};
    EXPECT_EQ(task17::count_long_strings(v, 0), 3u);
}
