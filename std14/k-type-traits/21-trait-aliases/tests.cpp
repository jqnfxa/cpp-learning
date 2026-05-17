#include <gtest/gtest.h>

#include <type_traits>

#include "solution.hpp"

namespace t = task21;

TEST(trait_aliases, bare_type_strips_reference)
{
    using R = t::bare_type<int &>;
    EXPECT_TRUE((std::is_same<R, int>::value));
}

TEST(trait_aliases, bare_type_strips_const)
{
    using R = t::bare_type<const int>;
    EXPECT_TRUE((std::is_same<R, int>::value));
}

TEST(trait_aliases, bare_type_strips_volatile)
{
    using R = t::bare_type<volatile int>;
    EXPECT_TRUE((std::is_same<R, int>::value));
}

TEST(trait_aliases, bare_type_strips_const_reference)
{
    using R = t::bare_type<const int &>;
    EXPECT_TRUE((std::is_same<R, int>::value));
}

TEST(trait_aliases, bare_type_strips_rvalue_reference)
{
    using R = t::bare_type<int &&>;
    EXPECT_TRUE((std::is_same<R, int>::value));
}

TEST(trait_aliases, bare_type_plain_int_unchanged)
{
    using R = t::bare_type<int>;
    EXPECT_TRUE((std::is_same<R, int>::value));
}

TEST(trait_aliases, bare_type_does_not_strip_pointer)
{
    /* The KEY test: bare_type strips ref+cv, NOT pointer.
     * The stub uses std::remove_pointer which gives `int`
     * for input `int*` -- but the reference's
     * remove_cv_t<remove_reference_t<int*>> leaves `int*`
     * alone. */
    using R = t::bare_type<int *>;
    EXPECT_TRUE((std::is_same<R, int *>::value));
}

TEST(trait_aliases, pick_true_yields_T)
{
    using R = t::pick<true, int, double>;
    EXPECT_TRUE((std::is_same<R, int>::value));
}

TEST(trait_aliases, pick_false_yields_U)
{
    using R = t::pick<false, int, double>;
    EXPECT_TRUE((std::is_same<R, double>::value));
}

TEST(trait_aliases, only_if_true_yields_T)
{
    using R = t::only_if<true, int>;
    EXPECT_TRUE((std::is_same<R, int>::value));
}

TEST(trait_aliases, is_bare_v_int)
{
    EXPECT_TRUE(t::is_bare_v<int>);
}

TEST(trait_aliases, is_bare_v_const_int)
{
    /* const int is NOT bare -- bare_type<const int> == int. */
    EXPECT_FALSE(t::is_bare_v<const int>);
}

TEST(trait_aliases, is_bare_v_int_pointer)
{
    /* int* IS bare -- bare_type<int*> == int*. The stub
     * incorrectly strips the pointer and gives int, so this
     * test catches the wrong trait. */
    EXPECT_TRUE(t::is_bare_v<int *>);
}
