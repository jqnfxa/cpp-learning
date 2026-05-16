#pragma once

#include <type_traits>

/*
 * Step 29 -- value categories. See task.md for the spec.
 *
 * Declare:
 *   enum class category { lvalue, xvalue, prvalue };
 *   template <class T> constexpr category classify();
 *
 * The function maps a type T to a category based on whether T is
 * an lvalue reference, rvalue reference, or neither.
 */

namespace task29
{

/* TODO: declare `enum class category { lvalue, xvalue, prvalue };` */

/* TODO: implement classify<T>() as a one-line constexpr returning
 * the matching category. Use std::is_lvalue_reference and
 * std::is_rvalue_reference. */

}
