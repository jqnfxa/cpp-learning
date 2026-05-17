#pragma once

/*
 * Step 2 -- constexpr member functions are no longer implicitly
 * const (C++14). See task.md for the spec.
 *
 * In C++11, every constexpr member function was implicitly const,
 * so writing a constexpr SETTER was impossible. C++14 dropped
 * that rule: `constexpr void set_x(int v) { x = v; }` is legal.
 *
 * The scaffold ships with setters that are correctly marked
 * `constexpr` but have EMPTY bodies -- so callers do not see
 * the assignment. The tests detect that.
 */

namespace task02
{

struct point
{
    int x;
    int y;

    constexpr point(int x_ = 0, int y_ = 0) noexcept
        : x(x_)
        , y(y_)
    {
    }

    /* TODO: assign v to x.
     *   constexpr void set_x(int v) noexcept { x = v; }
     */
    constexpr void set_x(int /*v*/) noexcept
    {
    }

    /* TODO: assign v to y.
     *   constexpr void set_y(int v) noexcept { y = v; }
     */
    constexpr void set_y(int /*v*/) noexcept
    {
    }

    constexpr int get_x() const noexcept
    {
        return x;
    }

    constexpr int get_y() const noexcept
    {
        return y;
    }
};

/*
 * Build a point at compile time via the constexpr setters. This
 * is the C++14-only pattern -- in C++11 the setters could not
 * mutate state at all (they would be implicitly const).
 */
constexpr point make_point(int x, int y) noexcept
{
    point p;
    p.set_x(x);
    p.set_y(y);
    return p;
}

}
