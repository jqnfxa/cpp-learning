#pragma once

#include <string>
#include <tuple>
#include <utility>

/*
 * Step 100 -- std::make_tuple / std::forward_as_tuple. See task.md.
 *
 * Two factories: one classic make_tuple, one piecewise pair
 * construction via two forward_as_tuple parameter packs.
 */

namespace task100
{

struct Point
{
    int x;
    int y;

    Point(int xx, int yy)
        : x(xx), y(yy)
    {
    }
};

struct Label
{
    std::string text;
    double weight;

    Label(std::string t, double w)
        : text(std::move(t)), weight(w)
    {
    }
};

/* TODO: return std::make_tuple(i, d, std::move(s)); */
inline std::tuple<int, double, std::string>
via_make_tuple(int /*i*/, double /*d*/, std::string /*s*/)
{
    return {};
}

/* TODO: return std::pair<Point, Label>(
 *     std::piecewise_construct,
 *     std::forward_as_tuple(px, py),
 *     std::forward_as_tuple(std::move(ltext), lweight));
 */
inline std::pair<Point, Label>
via_piecewise(int /*px*/, int /*py*/, std::string /*ltext*/, double /*lweight*/)
{
    return std::pair<Point, Label>(Point(0, 0), Label("", 0.0));
}

}
