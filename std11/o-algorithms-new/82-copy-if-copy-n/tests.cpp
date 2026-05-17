#include <gtest/gtest.h>

#include <vector>

#include "solution.hpp"

TEST(copy_if_copy_n, evens_empty)
{
    EXPECT_EQ(task82::evens({}), std::vector<int>{});
}

TEST(copy_if_copy_n, evens_no_evens)
{
    EXPECT_EQ(task82::evens({1, 3, 5, 7}), std::vector<int>{});
}

TEST(copy_if_copy_n, evens_all_evens)
{
    EXPECT_EQ(task82::evens({2, 4, 6, 8}), (std::vector<int>{2, 4, 6, 8}));
}

TEST(copy_if_copy_n, evens_mixed_preserves_order)
{
    EXPECT_EQ(task82::evens({1, 2, 3, 4, 5, 6}), (std::vector<int>{2, 4, 6}));
}

TEST(copy_if_copy_n, evens_handles_zero_and_negatives)
{
    /* `-2 % 2 == 0` in C99/C++11 truncated division. Zero is even. */
    EXPECT_EQ(task82::evens({0, -1, -2, -3, -4}), (std::vector<int>{0, -2, -4}));
}

TEST(copy_if_copy_n, first_n_basic)
{
    EXPECT_EQ(task82::first_n({1, 2, 3, 4, 5}, 3), (std::vector<int>{1, 2, 3}));
}

TEST(copy_if_copy_n, first_n_zero)
{
    EXPECT_EQ(task82::first_n({1, 2, 3}, 0), std::vector<int>{});
}

TEST(copy_if_copy_n, first_n_equal_to_size)
{
    EXPECT_EQ(task82::first_n({1, 2, 3}, 3), (std::vector<int>{1, 2, 3}));
}

TEST(copy_if_copy_n, first_n_larger_than_size_is_clamped)
{
    /* Without clamping this would read past the end -- UB. The
     * reference impl clamps k to v.size(). */
    EXPECT_EQ(task82::first_n({1, 2, 3}, 100), (std::vector<int>{1, 2, 3}));
}

TEST(copy_if_copy_n, first_n_on_empty_source)
{
    EXPECT_EQ(task82::first_n({}, 5), std::vector<int>{});
}

TEST(copy_if_copy_n, first_n_one_element)
{
    EXPECT_EQ(task82::first_n({42, 43, 44}, 1), std::vector<int>{42});
}
