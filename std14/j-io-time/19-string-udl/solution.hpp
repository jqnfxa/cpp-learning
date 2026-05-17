#pragma once

#include <string>

/*
 * Step 19 -- the `s` string UDL (C++14). See task.md.
 *
 * The `""s` suffix turns a string literal into `std::string`:
 *
 *   "hello"s   ==  std::string("hello")
 *
 * Without the suffix, `"hello"` is `const char *` and `"a" +
 * "b"` is pointer arithmetic, not concatenation. With the
 * suffix, `"a"s + "b"` is `std::string + const char*`, which
 * works as expected.
 *
 * The scaffold ships with stubs that DON'T use the suffix --
 * the function bodies build std::string awkwardly via explicit
 * construction. Tests still verify correctness, so the stub
 * happens to give the right answer; the pedagogical point is
 * the syntax change.
 *
 * To make the stub VISIBLY wrong, the stub also returns
 * truncated strings (forgets the second concatenation step).
 */

namespace task19
{

using namespace std::string_literals;

/*
 * Return the string "hello world" using the UDL form
 * `"hello"s + " world"`.
 */
inline std::string hello_world()
{
    /* TODO: return "hello"s + " world";
     *
     * Stub builds an std::string and forgets the " world"
     * suffix.
     */
    return std::string("hello");
}

/*
 * Return the greeting `"Hello, NAME!"` using `s`-suffix
 * concatenation throughout.
 */
inline std::string greeting(const std::string &name)
{
    /* TODO: return "Hello, "s + name + "!";
     *
     * Stub forgets the "!" suffix.
     */
    return std::string("Hello, ") + name;
}

/*
 * Return the size of the string `"hello"s`. With the UDL form
 * this is 5 (the std::string's size). Without (just `"hello"`),
 * `sizeof("hello")` would be 6 (includes the null terminator).
 *
 * This function exists to pin the "s-suffix yields a real
 * std::string" property.
 */
inline std::size_t size_of_hello()
{
    /* TODO: return "hello"s.size(); */
    return sizeof("hello"); /* stub: 6, includes null byte */
}

}
