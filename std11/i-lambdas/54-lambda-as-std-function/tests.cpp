#include <gtest/gtest.h>

#include <functional>
#include <vector>

#include "solution.hpp"

TEST(lambda_as_function, make_multiplier_basic)
{
    auto mul3 = task54::make_multiplier(3);
    EXPECT_EQ(mul3(5), 15);
    EXPECT_EQ(mul3(0), 0);
    EXPECT_EQ(mul3(-2), -6);
}

TEST(lambda_as_function, make_multiplier_zero)
{
    auto mul0 = task54::make_multiplier(0);
    EXPECT_EQ(mul0(99), 0);
    EXPECT_EQ(mul0(-99), 0);
}

TEST(lambda_as_function, make_multiplier_identity)
{
    auto mul1 = task54::make_multiplier(1);
    EXPECT_EQ(mul1(42), 42);
    EXPECT_EQ(mul1(-42), -42);
}

TEST(lambda_as_function, apply_all_empty)
{
    std::vector<std::function<int(int)>> empty;
    EXPECT_EQ(task54::apply_all(empty, 10), 10);
}

TEST(lambda_as_function, apply_all_single)
{
    std::vector<std::function<int(int)>> v;
    v.push_back(task54::make_multiplier(2));
    EXPECT_EQ(task54::apply_all(v, 10), 20);
}

TEST(lambda_as_function, apply_all_chain_of_multipliers)
{
    std::vector<std::function<int(int)>> v;
    v.push_back(task54::make_multiplier(2));
    v.push_back(task54::make_multiplier(3));
    /* 10 * 2 * 3 = 60 */
    EXPECT_EQ(task54::apply_all(v, 10), 60);
}

TEST(lambda_as_function, apply_all_mixed_capturing_and_captureless)
{
    /*
     * The vector holds:
     *   [0] capturing lambda (multiply by 2) via make_multiplier
     *   [1] captureless lambda (add 1) inline
     *
     * Both fit in std::function<int(int)>. Captureless one would
     * also fit a function pointer; the capturing one would NOT.
     * std::function bridges both.
     */
    std::vector<std::function<int(int)>> v;
    v.push_back(task54::make_multiplier(2));
    v.push_back([](int x)
                { return x + 1; });

    /* 10 -> 10 * 2 = 20 -> 20 + 1 = 21 */
    EXPECT_EQ(task54::apply_all(v, 10), 21);
}

TEST(lambda_as_function, apply_all_three_steps)
{
    std::vector<std::function<int(int)>> v;
    v.push_back([](int x)
                { return x + 1; });           /* +1 */
    v.push_back(task54::make_multiplier(10)); /* *10 */
    v.push_back([](int x)
                { return x - 3; }); /* -3 */

    /* 5 -> 6 -> 60 -> 57 */
    EXPECT_EQ(task54::apply_all(v, 5), 57);
}
