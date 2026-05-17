#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

using task63::int_or_string;

TEST(int_or_string, holds_int)
{
    int_or_string v(42);
    ASSERT_EQ(v.which(), int_or_string::is_int);
    EXPECT_EQ(v.as_int(), 42);
}

TEST(int_or_string, holds_string)
{
    int_or_string v(std::string("hello"));
    ASSERT_EQ(v.which(), int_or_string::is_string);
    EXPECT_EQ(v.as_string(), "hello");
}

TEST(int_or_string, holds_long_string)
{
    /* Long enough to bust the small-string optimisation -- forces a real
     * heap allocation, which is what we need to know placement-new and
     * the explicit destructor are actually being called. */
    const std::string big(256, 'x');
    int_or_string v(big);
    ASSERT_EQ(v.which(), int_or_string::is_string);
    EXPECT_EQ(v.as_string(), big);
}

TEST(int_or_string, copy_int)
{
    int_or_string a(7);
    int_or_string b(a);
    ASSERT_EQ(b.which(), int_or_string::is_int);
    EXPECT_EQ(b.as_int(), 7);
}

TEST(int_or_string, copy_string)
{
    int_or_string a(std::string("copied"));
    int_or_string b(a);
    ASSERT_EQ(b.which(), int_or_string::is_string);
    EXPECT_EQ(b.as_string(), "copied");
    /* Independent copies: mutate source, dest unchanged. */
    a.assign(std::string("mutated"));
    EXPECT_EQ(b.as_string(), "copied");
}

TEST(int_or_string, copy_long_string_is_deep)
{
    const std::string big(512, 'y');
    int_or_string a(big);
    int_or_string b(a);
    ASSERT_EQ(b.which(), int_or_string::is_string);
    EXPECT_EQ(b.as_string(), big);
    EXPECT_NE(&a.as_string(), &b.as_string());
}

TEST(int_or_string, assign_int_to_int)
{
    int_or_string v(1);
    v.assign(2);
    ASSERT_EQ(v.which(), int_or_string::is_int);
    EXPECT_EQ(v.as_int(), 2);
}

TEST(int_or_string, assign_string_to_string)
{
    int_or_string v(std::string("first"));
    v.assign(std::string("second"));
    ASSERT_EQ(v.which(), int_or_string::is_string);
    EXPECT_EQ(v.as_string(), "second");
}

TEST(int_or_string, switch_int_to_string)
{
    int_or_string v(99);
    v.assign(std::string("now a string"));
    ASSERT_EQ(v.which(), int_or_string::is_string);
    EXPECT_EQ(v.as_string(), "now a string");
}

TEST(int_or_string, switch_string_to_int)
{
    int_or_string v(std::string("now an int"));
    v.assign(123);
    ASSERT_EQ(v.which(), int_or_string::is_int);
    EXPECT_EQ(v.as_int(), 123);
}

TEST(int_or_string, switch_long_string_to_int_then_back)
{
    /* If the destructor on the old string branch is missed, this leaks.
     * If placement-new on the new string branch is missed, the read is UB
     * and typically crashes. Sanitisers would flag both. */
    int_or_string v(std::string(1024, 'z'));
    v.assign(0);
    ASSERT_EQ(v.which(), int_or_string::is_int);
    v.assign(std::string(1024, 'w'));
    ASSERT_EQ(v.which(), int_or_string::is_string);
    EXPECT_EQ(v.as_string(), std::string(1024, 'w'));
}
