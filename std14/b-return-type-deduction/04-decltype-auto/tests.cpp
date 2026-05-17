#include <gtest/gtest.h>

#include <type_traits>
#include <vector>

#include "solution.hpp"

namespace t = task04;

TEST(decltype_auto, auto_first_returns_value)
{
    std::vector<int> v{10, 20, 30};
    EXPECT_EQ(t::auto_first(v), 10);
}

TEST(decltype_auto, decltype_first_returns_value)
{
    std::vector<int> v{10, 20, 30};
    EXPECT_EQ(t::decltype_first(v), 10);
}

TEST(decltype_auto, auto_first_type_is_int)
{
    /* `auto` strips the reference: v[0] is int&, but auto_first
     * deduces `int`. */
    std::vector<int> v{1};
    using R = decltype(t::auto_first(v));
    EXPECT_TRUE((std::is_same<R, int>::value));
}

TEST(decltype_auto, decltype_first_type_is_int_ref)
{
    /* decltype(auto) follows decltype rules and KEEPS the int&
     * that v[0] yields. This test fails on the stub (which uses
     * bare auto). */
    std::vector<int> v{1};
    using R = decltype(t::decltype_first(v));
    EXPECT_TRUE((std::is_same<R, int &>::value));
}

TEST(decltype_auto, decltype_first_is_not_plain_int)
{
    /* Belt-and-braces inverse: plain int means "wrong, you
     * lost the reference." */
    std::vector<int> v{1};
    using R = decltype(t::decltype_first(v));
    EXPECT_FALSE((std::is_same<R, int>::value));
}

TEST(decltype_auto, decltype_first_aliases_source_address)
{
    /* If decltype_first really returns int&, then binding a
     * const-ref to it gives us the SAME address as &v[0]. With
     * the bare-auto stub, decltype_first returns a temporary
     * int and the const-ref lifetime-extends THAT temporary;
     * its address won't match &v[0]. */
    std::vector<int> v{42, 99};
    const auto &r = t::decltype_first(v);
    EXPECT_EQ(&r, &v[0]);
}

TEST(decltype_auto, auto_first_does_not_alias_source)
{
    /* Sanity: auto_first ALWAYS returns by value, regardless of
     * what we do in the body, so a const-ref bound to the call
     * extends a temporary -- never the same address as v[0]. */
    std::vector<int> v{42, 99};
    const auto &r = t::auto_first(v);
    EXPECT_NE(&r, &v[0]);
}

TEST(decltype_auto, works_for_string_vector)
{
    /* The reference preservation works for any T. */
    std::vector<std::string> v{"first", "second"};
    using R = decltype(t::decltype_first(v));
    EXPECT_TRUE((std::is_same<R, std::string &>::value));
    EXPECT_EQ(t::decltype_first(v), "first");
}

TEST(decltype_auto, address_aliases_for_string)
{
    std::vector<std::string> v{"alpha", "beta"};
    const auto &r = t::decltype_first(v);
    EXPECT_EQ(&r, &v[0]);
}

TEST(decltype_auto, mutation_through_reference_propagates)
{
    /* The textbook payoff: if decltype_first returns a real
     * reference, an alias bound to it observes mutations on
     * v[0]. The stub's by-value return makes `r` an independent
     * copy that never updates. */
    std::vector<int> v{0, 1, 2};
    const auto &r = t::decltype_first(v);
    v[0] = 1234;
    EXPECT_EQ(r, 1234);
}
