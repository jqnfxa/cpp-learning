#include <gtest/gtest.h>

#include <utility>

#include "solution.hpp"

/*
 * decltype((expr)) -- with the extra parens -- encodes the value
 * category of `expr`:
 *   lvalue   -> T &
 *   xvalue   -> T &&
 *   prvalue  -> T  (no reference)
 *
 * We feed it into task29::classify<T>() and assert at compile time.
 */

/*
 * Named variables and references at namespace scope.
 * (We can't put `int x = 0` then `decltype((x))` at namespace scope
 * easily, so we use a function returning a reference where needed.)
 */
namespace
{

int &lvalue_ref();  /* returns lvalue */
int &&xvalue_ref(); /* returns xvalue */
int prvalue_int();  /* returns prvalue */

}

/* Calls to lvalue_ref / xvalue_ref / prvalue_int give us neat
 * expression categories at namespace scope. */
static_assert(task29::classify<decltype((lvalue_ref()))>() == task29::category::lvalue, "function returning T & is lvalue");
static_assert(task29::classify<decltype((xvalue_ref()))>() == task29::category::xvalue, "function returning T && is xvalue");
static_assert(task29::classify<decltype((prvalue_int()))>() == task29::category::prvalue, "function returning T is prvalue");

/* Literals are prvalues. */
static_assert(task29::classify<decltype((42))>() == task29::category::prvalue, "integer literal is prvalue");
static_assert(task29::classify<decltype((3.14))>() == task29::category::prvalue, "float literal is prvalue");

/* std::move on a function call gives xvalue. */
static_assert(task29::classify<decltype((std::move(prvalue_int())))>() == task29::category::xvalue, "std::move yields xvalue");

TEST(value_categories, named_variable_is_lvalue)
{
    int x = 0;
    /* x as a named expression is an lvalue. */
    static_assert(task29::classify<decltype((x))>() == task29::category::lvalue, "named int is lvalue");
    (void)x;
    SUCCEED();
}

TEST(value_categories, named_rvalue_reference_variable_is_lvalue)
{
    /*
     * The classic gotcha: rr is declared T && (rvalue reference type),
     * but used as a *name*, the expression `rr` is an lvalue. This is
     * why std::move(rr) is needed when forwarding rvalues through a
     * function.
     */
    int &&rr = 7;
    static_assert(task29::classify<decltype((rr))>() == task29::category::lvalue, "named T&& variable is lvalue when used");
    (void)rr;
    SUCCEED();
}

TEST(value_categories, std_move_named_variable_is_xvalue)
{
    int x = 0;
    static_assert(task29::classify<decltype((std::move(x)))>() == task29::category::xvalue, "std::move(x) is xvalue");
    (void)x;
    SUCCEED();
}

TEST(value_categories, prvalue_constructor_call_is_prvalue)
{
    /* `int{}` is a prvalue temporary. */
    static_assert(task29::classify<decltype((int{}))>() == task29::category::prvalue, "T{} is prvalue");
    SUCCEED();
}

TEST(value_categories, enum_values_are_distinct)
{
    /* Sanity check on the runtime values, not just the static checks. */
    EXPECT_NE(task29::category::lvalue, task29::category::xvalue);
    EXPECT_NE(task29::category::xvalue, task29::category::prvalue);
    EXPECT_NE(task29::category::prvalue, task29::category::lvalue);
}
