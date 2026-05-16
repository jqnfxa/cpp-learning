#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <utility>

#include "solution.hpp"

TEST(std_move_swap, swap_ints)
{
    int a = 1;
    int b = 2;
    task31::swap(a, b);
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 1);
}

TEST(std_move_swap, swap_strings)
{
    std::string a = "alpha";
    std::string b = "beta";
    task31::swap(a, b);
    EXPECT_EQ(a, "beta");
    EXPECT_EQ(b, "alpha");
}

TEST(std_move_swap, swap_move_only_unique_ptr)
{
    /*
     * If the student's swap used copies (`T tmp = a;`) instead of
     * std::move, this test would not compile -- std::unique_ptr has
     * a deleted copy constructor. The fact that this builds and
     * passes proves the implementation uses std::move.
     */
    auto a = std::unique_ptr<int>(new int(7));
    auto b = std::unique_ptr<int>(new int(42));

    int *pa = a.get();
    int *pb = b.get();

    task31::swap(a, b);

    EXPECT_EQ(a.get(), pb);
    EXPECT_EQ(b.get(), pa);
    EXPECT_EQ(*a, 42);
    EXPECT_EQ(*b, 7);
}

TEST(std_move_swap, self_swap_is_a_no_op)
{
    int x = 5;
    task31::swap(x, x);
    EXPECT_EQ(x, 5);
}

TEST(std_move_swap, self_swap_string)
{
    std::string s = "hello";
    task31::swap(s, s);
    /* Standard library types handle self-move-assign cleanly. */
    EXPECT_EQ(s, "hello");
}
