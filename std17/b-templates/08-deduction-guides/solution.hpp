#pragma once

#include <string>
#include <utility>

/*
 * Step 8 -- user-defined deduction guides (C++17). See task.md.
 *
 * For a class template, the compiler synthesizes deduction
 * guides from each constructor. Sometimes you want a guide
 * that maps inputs differently -- e.g., "if the user passes
 * a const char*, deduce wrapper<std::string>, not
 * wrapper<const char*>."
 *
 * User-defined deduction guides have the syntax:
 *
 *   template <...> ClassName(args) -> ClassName<T>;
 *
 * The "->" is part of the syntax (NOT a function pointer).
 * Live OUTSIDE the class definition, at namespace scope.
 *
 * The scaffold ships WITHOUT the user-defined guide. CTAD then
 * uses the implicit guide from the constructor:
 *
 *   wrapper w("hello");   // implicit: wrapper<const char*>
 *
 * Tests verify the user-defined guide is in effect by checking
 * that the deduced type is wrapper<std::string>, not
 * wrapper<const char*>.
 */

namespace task08
{

template <class T>
struct wrapper
{
    T value;

    wrapper(T v) noexcept
        : value(std::move(v))
    {
    }
};

/* TODO: add a user-defined deduction guide that maps
 * `wrapper(const char*)` to `wrapper<std::string>`:
 *
 *   wrapper(const char *) -> wrapper<std::string>;
 *
 * Without this, `wrapper w("hello")` deduces `wrapper<const
 * char *>`, not `wrapper<std::string>`. */

/*
 * A helper that exercises CTAD on wrapper with a string
 * literal. Returns the wrapped value.
 *
 * With the user-defined guide, `auto w = wrapper("hello");`
 * deduces `wrapper<std::string>` and `w.value` is a
 * std::string.
 *
 * Reference: returns the wrapped std::string("hello").
 * Stub: returns wrapper<const char*>.value -- the pointer
 * (not the string), which we coerce to a length-comparison
 * to keep the test focused on the deduced TYPE.
 */
inline std::string wrap_literal()
{
    /* TODO:
     *   wrapper w("hello");
     *   return w.value;
     *
     * Without the user-defined guide, w.value is `const char*`
     * -- the test still compiles but the type-check test
     * fails. */
    return std::string{};
}

}
