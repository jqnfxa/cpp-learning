#pragma once

#include <memory>

/*
 * Step 70 -- std::make_shared. See task.md for the spec.
 *
 * Three factories side by side so the tests can compare allocation
 * counts and argument forwarding behaviour.
 */

namespace task70
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

/* TODO: return std::shared_ptr<int>(new int(n));  -- two allocations. */
inline std::shared_ptr<int> make_via_new(int /*n*/)
{
    return std::shared_ptr<int>();
}

/* TODO: return std::make_shared<int>(n);  -- one allocation. */
inline std::shared_ptr<int> make_via_make_shared(int /*n*/)
{
    return std::shared_ptr<int>();
}

/* TODO: return std::make_shared<Point>(x, y);  -- forwarded args. */
inline std::shared_ptr<Point> make_point(int /*x*/, int /*y*/)
{
    return std::shared_ptr<Point>();
}

}
