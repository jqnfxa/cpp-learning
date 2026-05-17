#pragma once

#include <cstddef>
#include <memory>
#include <string>

/*
 * Step 12 -- std::make_unique (C++14). See task.md.
 *
 * Three functions:
 *   - make_widget: object form, forwards a single int to widget's ctor.
 *   - make_string_n: object form, forwards (count, char) to string's ctor.
 *   - make_array: array form, value-initialises N ints to 0.
 *
 * Scaffold ships with nullptr returns; tests catch them.
 */

namespace task12
{

struct widget
{
    int x;

    explicit widget(int x_)
        : x(x_)
    {
    }
};

/* TODO: return std::make_unique<widget>(value);
 *
 * The C++14 equivalent of `std::unique_ptr<widget>(new widget(value))`.
 * Less typing, no leak window when used inside a compound
 * expression with another allocation. */
inline std::unique_ptr<widget> make_widget(int /*value*/)
{
    return nullptr;
}

/* TODO: return std::make_unique<std::string>(count, c);
 *
 * make_unique forwards ALL its args to the constructor of T, so
 * the `(count, c)` form here invokes std::string's (size_t, char)
 * ctor for "ccccc..." */
inline std::unique_ptr<std::string> make_string_n(std::size_t /*count*/, char /*c*/)
{
    return nullptr;
}

/* TODO: return std::make_unique<int[]>(size);
 *
 * The ARRAY form of make_unique takes a size argument and
 * value-initialises all elements. There's no equivalent forwarding
 * form for arrays in C++14 (you can't pass an initialiser-list
 * through make_unique<T[]>). */
inline std::unique_ptr<int[]> make_array(std::size_t /*size*/)
{
    return nullptr;
}

}
