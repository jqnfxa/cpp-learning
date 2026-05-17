#include <gtest/gtest.h>

#include <string>
#include <type_traits>

#include "solution.hpp"

/* Plain types are unchanged. */
static_assert(std::is_same<task102::bare_type<int>::type, int>::value,
              "bare_type<int> must be int");
static_assert(std::is_same<task102::bare_type<double>::type, double>::value,
              "bare_type<double> must be double");
static_assert(std::is_same<task102::bare_type<std::string>::type, std::string>::value,
              "bare_type<std::string> must be std::string");

/* References are stripped. */
static_assert(std::is_same<task102::bare_type<int &>::type, int>::value,
              "bare_type<int &> must be int");
static_assert(std::is_same<task102::bare_type<int &&>::type, int>::value,
              "bare_type<int &&> must be int");

/* Top-level cv is stripped. */
static_assert(std::is_same<task102::bare_type<const int>::type, int>::value,
              "bare_type<const int> must be int");
static_assert(std::is_same<task102::bare_type<volatile int>::type, int>::value,
              "bare_type<volatile int> must be int");
static_assert(std::is_same<task102::bare_type<const volatile int>::type, int>::value,
              "bare_type<const volatile int> must be int");

/* Reference + cv together. */
static_assert(std::is_same<task102::bare_type<const int &>::type, int>::value,
              "bare_type<const int &> must be int");
static_assert(std::is_same<task102::bare_type<volatile int &&>::type, int>::value,
              "bare_type<volatile int &&> must be int");
static_assert(std::is_same<task102::bare_type<const volatile int &>::type, int>::value,
              "bare_type<const volatile int &> must be int");

/* Pointers: top-level const on the *pointer* is stripped;
 * const on the *pointee* survives. */
static_assert(std::is_same<task102::bare_type<int *const>::type, int *>::value,
              "bare_type<int *const> must be int *");
static_assert(std::is_same<task102::bare_type<const int *>::type, const int *>::value,
              "bare_type<const int *> must be const int *");

/* Arrays survive (unlike std::decay). */
static_assert(std::is_same<task102::bare_type<int[5]>::type, int[5]>::value,
              "bare_type<int[5]> must remain int[5], not decay to int *");

/* Functions survive (unlike std::decay). */
using fn_t = int(int);
static_assert(std::is_same<task102::bare_type<fn_t>::type, fn_t>::value,
              "bare_type<fn_t> must remain fn_t, not decay to a pointer");

/* Const reference to a class type. */
static_assert(
    std::is_same<task102::bare_type<const std::string &>::type, std::string>::value,
    "bare_type<const std::string &> must be std::string");

TEST(transformation_traits, smoke)
{
    /* Validation is via the static_asserts above; this test exists
     * just so ctest has a runtime artifact to run. */
    SUCCEED();
}
