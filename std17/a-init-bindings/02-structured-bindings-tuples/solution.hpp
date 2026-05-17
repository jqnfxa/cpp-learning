#pragma once

#include <map>
#include <string>
#include <tuple>
#include <utility>

/*
 * Step 2 -- structured bindings on tuples / pairs (C++17).
 * See task.md.
 *
 * Tuple-like types satisfy the `std::tuple_size` +
 * `std::get<I>` protocol. Structured bindings detect this and
 * destructure via the tuple-like path.
 *
 * Three demos:
 *   - try_insert: split std::map::insert's pair<iterator, bool>.
 *   - pair_sum: destructure a std::pair<int, int>.
 *   - tuple_summary: destructure a std::tuple<int, string, double>.
 *
 * The scaffold ships with stubs that return wrong values.
 */

namespace task02
{

/*
 * Try to insert (k, v) into m. Returns true iff the key
 * was newly inserted (false if it already existed).
 *
 * std::map::insert returns std::pair<iterator, bool> -- the
 * canonical motivating case for structured bindings.
 */
inline bool try_insert(std::map<int, int> &m, int k, int v)
{
    /* TODO:
     *   auto [it, inserted] = m.insert({k, v});
     *   (void)it;
     *   return inserted;
     *
     * The stub doesn't actually insert. */
    (void)m;
    (void)k;
    (void)v;
    return false;
}

/*
 * Destructure a pair and return the sum.
 */
inline int pair_sum(std::pair<int, int> p)
{
    /* TODO:
     *   auto [a, b] = p;
     *   return a + b;
     */
    (void)p;
    return 0;
}

/*
 * Destructure a tuple of (int, string, double) and return
 * a formatted summary string.
 */
inline std::string tuple_summary(std::tuple<int, std::string, double> t)
{
    /* TODO:
     *   auto [age, name, score] = t;
     *   (void)score;
     *   return name + ":" + std::to_string(age);
     */
    (void)t;
    return "";
}

/*
 * Destructure the tuple and return just the score (the
 * double field).
 */
inline double tuple_score(std::tuple<int, std::string, double> t)
{
    /* TODO:
     *   auto [age, name, score] = t;
     *   (void)age;
     *   (void)name;
     *   return score;
     */
    (void)t;
    return 0.0;
}

}
