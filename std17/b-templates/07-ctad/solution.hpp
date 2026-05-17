#pragma once

#include <string>
#include <tuple>
#include <utility>
#include <vector>

/*
 * Step 7 -- class template argument deduction (CTAD) (C++17).
 * See task.md.
 *
 * Before C++17, instantiating a class template required
 * explicit template arguments:
 *
 *   std::pair<int, double> p(1, 2.5);
 *   std::vector<int> v{1, 2, 3};
 *
 * Or a factory function (`std::make_pair`) hand-deduced them.
 *
 * C++17's CTAD lets the compiler deduce them from the
 * constructor arguments:
 *
 *   std::pair p(1, 2.5);              // deduces pair<int, double>
 *   std::vector v{1, 2, 3};           // deduces vector<int>
 *
 * Three demo functions that use CTAD internally and return
 * the constructed object. The scaffold uses default-
 * constructed sentinels instead.
 */

namespace task07
{

/*
 * Build and return a `std::pair` via CTAD (no explicit
 * template arguments). Deduces from the constructor args.
 *
 * Reference: `std::pair p(a, b); return p;`
 * Stub: default-construct an empty pair<int, int>.
 */
inline std::pair<int, double> make_pair_via_ctad(int a, double b)
{
    /* TODO:
     *   std::pair p(a, b);
     *   return p;
     */
    (void)a;
    (void)b;
    return std::pair<int, double>{};
}

/*
 * Build and return a `std::vector` via CTAD on an
 * initializer-list constructor.
 *
 * Reference: `std::vector v{a, b, c}; return v;`
 * Stub: default-construct an empty vector.
 */
inline std::vector<int> make_vector_via_ctad(int a, int b, int c)
{
    /* TODO:
     *   std::vector v{a, b, c};
     *   return v;
     */
    (void)a;
    (void)b;
    (void)c;
    return std::vector<int>{};
}

/*
 * Build a tuple via CTAD with three different types.
 *
 * Reference: `std::tuple t{x, name, score}; return t;`
 * Stub: default-construct.
 */
inline std::tuple<int, std::string, double>
make_tuple_via_ctad(int x, std::string name, double score)
{
    /* TODO:
     *   std::tuple t{x, std::move(name), score};
     *   return t;
     */
    (void)x;
    (void)name;
    (void)score;
    return std::tuple<int, std::string, double>{};
}

}
