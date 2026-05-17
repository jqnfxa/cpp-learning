#pragma once

#include <functional>

/*
 * Step 13 -- lambda capture of *this (C++17). See task.md.
 *
 * C++11/14 supported `[this]` (captures the pointer) and
 * `[&]` (implicit by-ref). Neither survives the enclosing
 * object's destruction.
 *
 * C++17 added `[*this]` which COPIES the enclosing object
 * into the closure. The closure owns its own copy of `*this`,
 * independent of the original.
 *
 * The scaffold uses `[this]` (pointer capture). Tests
 * detect: after mutating the original object, the closure
 * should return the captured-time value, not the new one.
 */

namespace task13
{

class widget
{
    int x_;

public:
    explicit widget(int v) noexcept
        : x_(v)
    {
    }

    int get() const noexcept
    {
        return x_;
    }

    void set(int v) noexcept
    {
        x_ = v;
    }

    /*
     * Return a callable that yields the widget's value at
     * the time of capture.
     *
     * Reference: `[*this]() { return x_; }` -- by-value
     *            copy of *this; closure has its own x_.
     * Stub: `[this]() { return x_; }` -- pointer capture;
     *       closure dereferences the original's x_.
     */
    std::function<int()> make_getter() const
    {
        /* TODO: capture *this by value:
         *   return [*this]() { return x_; };
         *
         * The stub captures `this` by pointer; tests verify
         * that after `set(999)` on the original, the getter
         * still returns the original value -- which only
         * works for by-value capture. */
        return [this]() { return x_; };
    }
};

}
