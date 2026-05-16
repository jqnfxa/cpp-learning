#pragma once

/*
 * Step 40 -- literal types + constexpr ctors. See task.md.
 *
 * Implement point as a literal type with a constexpr ctor + two
 * constexpr accessors. Then implement distance_squared as a free
 * constexpr function.
 */

namespace task40
{

class point
{
public:
    /* TODO: constexpr point(int x, int y) : x_(x), y_(y) {} */
    point(int /* x */, int /* y */)
        : x_(0), y_(0)
    {
    }

    /* TODO: constexpr int x() const { return x_; } */
    int x() const
    {
        return x_;
    }

    /* TODO: constexpr int y() const { return y_; } */
    int y() const
    {
        return y_;
    }

private:
    int x_;
    int y_;
};

/* TODO: constexpr int distance_squared(point a, point b) {
 *   return (a.x()-b.x())*(a.x()-b.x()) + (a.y()-b.y())*(a.y()-b.y());
 * } */
inline int distance_squared(point /* a */, point /* b */)
{
    return 0;
}

}
