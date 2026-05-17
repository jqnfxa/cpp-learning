#pragma once

#include <string>

/*
 * Step 3 -- structured bindings on structs (C++17). See task.md.
 *
 * The third kind of structured binding: any class type with
 * all-public non-static data members. The bindings are taken
 * in DECLARATION ORDER.
 *
 *   struct point { int x, y; };
 *   auto [a, b] = make_point();   // a=point's x, b=point's y
 *
 * Two demos: a 2-field point and a 3-field profile (mixed
 * types). The scaffold ships with wrong values; tests detect.
 */

namespace task03
{

struct point
{
    int x;
    int y;
};

struct profile
{
    int age;
    std::string name;
    double score;
};

inline point make_point(int x, int y)
{
    return {x, y};
}

inline profile make_profile(int age, std::string name, double score)
{
    return {age, std::move(name), score};
}

/*
 * Destructure a point via `auto [a, b] = p;` and return the
 * sum of x and y.
 */
inline int point_sum(point p)
{
    /* TODO:
     *   auto [a, b] = p;
     *   return a + b;
     *
     * The stub returns 0.
     */
    (void)p;
    return 0;
}

/*
 * Destructure a point and return its components swapped.
 */
inline point point_swap(point p)
{
    /* TODO:
     *   auto [a, b] = p;
     *   return {b, a};
     *
     * The stub returns the input unchanged.
     */
    return p;
}

/*
 * Destructure a profile and return a "name:age" summary
 * string, ignoring the score.
 */
inline std::string profile_label(profile p)
{
    /* TODO:
     *   auto [age, name, score] = p;
     *   (void)score;
     *   return name + ":" + std::to_string(age);
     *
     * The stub returns an empty string.
     */
    (void)p;
    return "";
}

/*
 * Return just the score field via structured binding.
 */
inline double profile_score(profile p)
{
    /* TODO:
     *   auto [age, name, score] = p;
     *   (void)age;
     *   (void)name;
     *   return score;
     */
    (void)p;
    return 0.0;
}

}
