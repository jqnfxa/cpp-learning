#include <gtest/gtest.h>

#include <climits>
#include <string>
#include <type_traits>
#include <vector>

#include "solution.hpp"

/* Compile-time identity of each alias. */
static_assert(std::is_same<task08::u64, unsigned long long>::value, "u64 must alias unsigned long long");
static_assert(std::is_same<task08::int_binary_op, int (*)(int, int)>::value, "int_binary_op must alias int(*)(int, int)");
static_assert(std::is_same<task08::string_vec, std::vector<std::string>>::value, "string_vec must alias std::vector<std::string>");
static_assert(std::is_same<task08::vec_int_iter, std::vector<int>::iterator>::value, "vec_int_iter must alias std::vector<int>::iterator");

/* Smoke tests that the aliases are actually usable like the underlying type. */

static int add_ints(int a, int b)
{
    return a + b;
}

TEST(type_alias, u64_holds_a_large_value)
{
    task08::u64 x = ULLONG_MAX;
    EXPECT_GT(x, static_cast<task08::u64>(1ULL << 32));
}

TEST(type_alias, int_binary_op_is_callable)
{
    task08::int_binary_op op = &add_ints;
    EXPECT_EQ(op(3, 4), 7);
}

TEST(type_alias, string_vec_is_a_vector_of_strings)
{
    task08::string_vec v = {"alpha", "beta", "gamma"};
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[1], "beta");
}

TEST(type_alias, vec_int_iter_is_an_iterator)
{
    std::vector<int> v = {10, 20, 30};
    task08::vec_int_iter it = v.begin();
    EXPECT_EQ(*it, 10);
    ++it;
    EXPECT_EQ(*it, 20);
}
