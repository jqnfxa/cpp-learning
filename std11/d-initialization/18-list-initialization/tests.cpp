#include <gtest/gtest.h>

#include <utility>
#include <vector>

#include "solution.hpp"

TEST(list_init, make_int_returns_argument)
{
    EXPECT_EQ(task18::make_int(0), 0);
    EXPECT_EQ(task18::make_int(42), 42);
    EXPECT_EQ(task18::make_int(-17), -17);
}

TEST(list_init, make_default_int_is_zero)
{
    /*
     * Value-initialized fundamental type is zero. This is the
     * difference between `int x;` (indeterminate at non-static
     * storage) and `int x{};` (always zero).
     */
    EXPECT_EQ(task18::make_default_int(), 0);
}

TEST(list_init, make_pair_of_holds_arguments)
{
    auto p = task18::make_pair_of(7, 11);
    EXPECT_EQ(p.first, 7);
    EXPECT_EQ(p.second, 11);
}

TEST(list_init, make_pair_of_negative_values)
{
    auto p = task18::make_pair_of(-1, -2);
    EXPECT_EQ(p.first, -1);
    EXPECT_EQ(p.second, -2);
}

TEST(list_init, make_vec3_three_elements)
{
    auto v = task18::make_vec3(10, 20, 30);
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(list_init, make_vec3_is_initializer_list_form_not_fill_form)
{
    /*
     * vector<int>(3, 0) is the fill constructor: 3 elements all 0.
     * vector<int>{3, 0} is the initializer_list constructor: 2
     * elements, 3 and 0.
     *
     * Asking for {3, 0, 0} from this function should produce a
     * 3-element vector [3, 0, 0]. If the student used parens by
     * mistake, the result would be a 3-element vector [0, 0, 0]
     * (fill, with 3 copies of 0) -- and our 3 != 0 check would
     * catch that.
     */
    auto v = task18::make_vec3(3, 0, 0);
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 3);
    EXPECT_EQ(v[1], 0);
    EXPECT_EQ(v[2], 0);
}
