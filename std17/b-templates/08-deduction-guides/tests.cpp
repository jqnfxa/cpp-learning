#include <gtest/gtest.h>

#include <string>
#include <type_traits>

#include "solution.hpp"

namespace t = task08;

TEST(deduction_guides, wrap_literal_returns_hello)
{
    EXPECT_EQ(t::wrap_literal(), "hello");
}

TEST(deduction_guides, direct_literal_deduces_string)
{
    /* The KEY test: with the user-defined guide,
     * `wrapper w("hi")` deduces `wrapper<std::string>`. */
    t::wrapper w("hi");
    EXPECT_TRUE(
        (std::is_same<decltype(w), t::wrapper<std::string>>::value));
}

TEST(deduction_guides, direct_literal_holds_string_value)
{
    t::wrapper w("abc");
    EXPECT_EQ(w.value, "abc");
}

TEST(deduction_guides, int_deduction_still_works)
{
    /* The implicit guide for `wrapper<int>` from the
     * constructor still applies; user-defined guides only
     * override the const-char* case. */
    t::wrapper w(42);
    EXPECT_TRUE(
        (std::is_same<decltype(w), t::wrapper<int>>::value));
    EXPECT_EQ(w.value, 42);
}

TEST(deduction_guides, double_deduction_still_works)
{
    t::wrapper w(3.14);
    EXPECT_TRUE(
        (std::is_same<decltype(w), t::wrapper<double>>::value));
    EXPECT_DOUBLE_EQ(w.value, 3.14);
}

TEST(deduction_guides, std_string_deduction_explicit)
{
    /* When the argument is a std::string already, the implicit
     * guide gives wrapper<std::string> -- matches our
     * user-defined guide's result. */
    t::wrapper w(std::string("from-string"));
    EXPECT_TRUE(
        (std::is_same<decltype(w), t::wrapper<std::string>>::value));
}

TEST(deduction_guides, multiple_literals)
{
    t::wrapper a("alpha");
    t::wrapper b("beta");
    EXPECT_EQ(a.value, "alpha");
    EXPECT_EQ(b.value, "beta");
    /* Both have type wrapper<std::string>. */
    EXPECT_TRUE((std::is_same<decltype(a), decltype(b)>::value));
}

TEST(deduction_guides, wrap_literal_type_is_string)
{
    /* The helper's RETURN type is std::string -- if the inner
     * `wrapper w("hello")` deduced wrapper<const char*>, the
     * stub's body (just returning std::string{}) is what
     * happens. The reference uses the user-defined guide to
     * get wrapper<std::string>, and w.value is the
     * std::string "hello". */
    auto result = t::wrap_literal();
    EXPECT_TRUE((std::is_same<decltype(result), std::string>::value));
}
