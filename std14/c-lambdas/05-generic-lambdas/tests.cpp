#include <gtest/gtest.h>

#include <string>
#include <type_traits>

#include "solution.hpp"

namespace t = task05;

TEST(generic_lambdas, square_int)
{
    EXPECT_EQ(t::square(5), 25);
}

TEST(generic_lambdas, square_int_negative)
{
    EXPECT_EQ(t::square(-7), 49);
}

TEST(generic_lambdas, square_double)
{
    EXPECT_DOUBLE_EQ(t::square(2.5), 6.25);
}

TEST(generic_lambdas, square_long)
{
    EXPECT_EQ(t::square(1000000L), 1000000000000L);
}

TEST(generic_lambdas, concat_ints)
{
    EXPECT_EQ(t::concat(2, 3), 5);
}

TEST(generic_lambdas, concat_doubles)
{
    EXPECT_DOUBLE_EQ(t::concat(1.5, 2.5), 4.0);
}

TEST(generic_lambdas, concat_strings)
{
    std::string h = "hello ";
    std::string w = "world";
    EXPECT_EQ(t::concat(h, w), std::string("hello world"));
}

TEST(generic_lambdas, concat_mixed_int_double)
{
    /* Heterogeneous parameter types: T1=int, T2=double. The
     * generic lambda instantiates with both, and the deduced
     * return type follows decltype(a+b). */
    EXPECT_DOUBLE_EQ(t::concat(2, 3.5), 5.5);
}

TEST(generic_lambdas, concat_returns_double_for_mixed)
{
    /* Type check via runtime is_same: the closure's operator()
     * was instantiated with (int, double) and the return type
     * is double. */
    auto r = t::concat(1, 2.5);
    EXPECT_TRUE((std::is_same<decltype(r), double>::value));
}

TEST(generic_lambdas, square_returns_int_for_int)
{
    auto r = t::square(3);
    EXPECT_TRUE((std::is_same<decltype(r), int>::value));
}

TEST(generic_lambdas, multiple_specializations_coexist)
{
    /* The same lambda object can be called with different
     * argument types -- each call site instantiates its own
     * template specialisation. */
    EXPECT_EQ(t::square(4), 16);
    EXPECT_DOUBLE_EQ(t::square(1.5), 2.25);
    EXPECT_EQ(t::concat(1, 2), 3);
    EXPECT_EQ(t::concat(std::string("a"), std::string("b")),
              std::string("ab"));
}
