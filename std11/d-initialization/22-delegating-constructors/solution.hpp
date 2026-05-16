#pragma once

/*
 * Step 22 -- delegating constructors. See task.md for the spec.
 *
 * Implement task22::point with three constructors:
 *   point()          -> delegate to point(0, 0)
 *   point(int v)     -> delegate to point(v, v)
 *   point(int x, int y)  -> the target; initializes x_ and y_
 *
 * Plus the two const accessors x() and y(). Do not change the
 * declared signatures.
 */

namespace task22
{

class point
{
public:
    /* TODO: delegate to point(0, 0). */
    point()
    {
    }

    /* TODO: delegate to point(v, v). */
    point(int /* v */)
    {
    }

    /* TODO: the target. Initialize x_ and y_. */
    point(int /* x */, int /* y */)
    {
    }

    int x() const
    {
        return x_;
    }

    int y() const
    {
        return y_;
    }

private:
    int x_ = 999;
    int y_ = 999;
};

}
