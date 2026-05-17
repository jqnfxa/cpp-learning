#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

TEST(property_traits, integers)
{
    EXPECT_EQ(task101::name_kind<int>(), "integral");
    EXPECT_EQ(task101::name_kind<long>(), "integral");
    EXPECT_EQ(task101::name_kind<long long>(), "integral");
    EXPECT_EQ(task101::name_kind<short>(), "integral");
    EXPECT_EQ(task101::name_kind<unsigned>(), "integral");
}

TEST(property_traits, char_kinds)
{
    EXPECT_EQ(task101::name_kind<char>(), "integral");
    EXPECT_EQ(task101::name_kind<signed char>(), "integral");
    EXPECT_EQ(task101::name_kind<unsigned char>(), "integral");
    EXPECT_EQ(task101::name_kind<wchar_t>(), "integral");
    EXPECT_EQ(task101::name_kind<char16_t>(), "integral");
    EXPECT_EQ(task101::name_kind<char32_t>(), "integral");
}

TEST(property_traits, bool_is_integral)
{
    /* The surprising one: bool is in the integral camp. */
    EXPECT_EQ(task101::name_kind<bool>(), "integral");
}

TEST(property_traits, floating_point)
{
    EXPECT_EQ(task101::name_kind<float>(), "floating_point");
    EXPECT_EQ(task101::name_kind<double>(), "floating_point");
    EXPECT_EQ(task101::name_kind<long double>(), "floating_point");
}

TEST(property_traits, pointers)
{
    EXPECT_EQ(task101::name_kind<int *>(), "pointer");
    EXPECT_EQ(task101::name_kind<const int *>(), "pointer");
    EXPECT_EQ(task101::name_kind<void *>(), "pointer");
    EXPECT_EQ(task101::name_kind<const char *>(), "pointer");
    EXPECT_EQ(task101::name_kind<std::string *>(), "pointer");
}

TEST(property_traits, other_class_types)
{
    EXPECT_EQ(task101::name_kind<std::string>(), "other");
}

TEST(property_traits, other_void)
{
    EXPECT_EQ(task101::name_kind<void>(), "other");
}

TEST(property_traits, other_arrays_and_refs)
{
    /* int[5] is "other" (it's an array, not a pointer despite the
     * decay relationship). */
    EXPECT_EQ(task101::name_kind<int[5]>(), "other");
    /* int& is "other" (it's a reference -- not integral or pointer in
     * its own right, even though removing the reference would
     * uncover an integral). */
    EXPECT_EQ(task101::name_kind<int &>(), "other");
}

TEST(property_traits, const_volatile_qualifications_still_classify)
{
    /* CV-qualified types fall in the same category as their bare
     * versions. */
    EXPECT_EQ(task101::name_kind<const int>(), "integral");
    EXPECT_EQ(task101::name_kind<volatile double>(), "floating_point");
    EXPECT_EQ(task101::name_kind<int *const>(), "pointer");
}
