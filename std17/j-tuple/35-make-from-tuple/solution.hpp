#pragma once

#include <tuple>

/*
 * Step 35 -- std::make_from_tuple (C++17). Construct T from
 * a tuple of constructor args.
 */

namespace task35
{

struct point
{
    int x;
    int y;

    constexpr point(int x_, int y_)
        : x(x_)
        , y(y_)
    {
    }
};

/* TODO: return std::make_from_tuple<point>(t); */
/* TODO: return std::make_from_tuple<point>(t); */
inline point make_point(std::tuple<int, int> t)
{
    (void)t;
    return point{0, 0};
}

}
