#include <gtest/gtest.h>

#include <string>
#include <type_traits>
#include <vector>

#include "solution.hpp"

/* element_t. */
static_assert(std::is_same<task62::element_t<std::vector<int>>, int &>::value, "vector<int>::*begin() -> int &");
static_assert(std::is_same<task62::element_t<const std::vector<int>>, const int &>::value, "const vector<int>::*begin() -> const int &");
static_assert(std::is_same<task62::element_t<std::vector<std::string>>, std::string &>::value, "vector<string>::*begin() -> string &");

/* sum_t. */
static_assert(std::is_same<task62::sum_t<int, int>, int>::value, "int + int -> int");
static_assert(std::is_same<task62::sum_t<int, double>, double>::value, "int + double -> double");
static_assert(std::is_same<task62::sum_t<long, int>, long>::value, "long + int -> long");
/* short + char both promote to int by the usual arithmetic conversions. */
static_assert(std::is_same<task62::sum_t<short, char>, int>::value, "short + char -> int (after promotion)");

/* declared_t. declval<T>() returns T&&, so declared_t<int> is int&&. */
static_assert(std::is_same<task62::declared_t<int>, int &&>::value, "declval<int>() yields int &&");
static_assert(std::is_same<task62::declared_t<int &>, int &>::value, "declval<int &>() yields int &");

TEST(decltype_aliases, smoke)
{
    /* The aliases are pure metaprogramming; this test exists
     * just so there's a runtime artifact to run. The real
     * verification is the static_asserts above. */
    SUCCEED();
}
