#include <gtest/gtest.h>

#include <cstddef>
#include <type_traits>

#include "solution.hpp"

namespace t = task10;

TEST(auto_nttp, fixed_array_size_5)
{
    EXPECT_EQ(t::fixed_array<5>::size(), 5u);
}

TEST(auto_nttp, fixed_array_size_10)
{
    EXPECT_EQ(t::fixed_array<10>::size(), 10u);
}

TEST(auto_nttp, fixed_array_size_ull)
{
    /* The KEY test: instantiating with an ULL constant.
     * With `template <auto N>`, N's type is `unsigned long
     * long`. The size() method's return type follows it.
     * With the stub's `template <size_t N>`, the ULL value
     * gets coerced to size_t and size()'s type is size_t. */
    auto s = t::fixed_array<10ULL>::size();
    EXPECT_TRUE((std::is_same<decltype(s), unsigned long long>::value));
}

TEST(auto_nttp, fixed_array_size_returns_int_for_int_literal)
{
    /* With `template <auto N>`, the literal `5` (int) makes N
     * an int. size() returns int. Stub returns size_t. */
    auto s = t::fixed_array<5>::size();
    EXPECT_TRUE((std::is_same<decltype(s), int>::value));
}

TEST(auto_nttp, fixed_array_data_access)
{
    /* The data array has the declared length. */
    t::fixed_array<3> arr{};
    arr.data[0] = 10;
    arr.data[1] = 20;
    arr.data[2] = 30;
    EXPECT_EQ(arr.data[0], 10);
    EXPECT_EQ(arr.data[2], 30);
}

TEST(auto_nttp, value_holder_int)
{
    EXPECT_EQ(t::value_holder<42>::value(), 42);
}

TEST(auto_nttp, value_holder_value_type_int)
{
    auto v = t::value_holder<42>::value();
    EXPECT_TRUE((std::is_same<decltype(v), int>::value));
}

TEST(auto_nttp, value_holder_long_long)
{
    /* With `template <auto V>`, the LL literal deduces V as
     * long long. Stub's `template <int V>` forces V to int,
     * losing the type information. */
    auto v = t::value_holder<42LL>::value();
    EXPECT_EQ(v, 42LL);
    EXPECT_TRUE((std::is_same<decltype(v), long long>::value));
}

TEST(auto_nttp, value_holder_char)
{
    /* Same idea with a char literal. */
    constexpr char c = 'A';
    auto v = t::value_holder<c>::value();
    EXPECT_EQ(v, 'A');
    EXPECT_TRUE((std::is_same<decltype(v), char>::value));
}
