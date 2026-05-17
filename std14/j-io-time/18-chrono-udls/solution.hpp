#pragma once

#include <chrono>

/*
 * Step 18 -- chrono UDLs (C++14). See task.md.
 *
 * C++14 added user-defined literals for the chrono duration
 * types in the std::chrono_literals namespace:
 *
 *   1ns, 1us, 1ms, 1s, 1min, 1h
 *
 * They expand to `std::chrono::nanoseconds(1)` etc., letting
 * you write durations in their natural notation.
 *
 * Five named constants and four "natural" wrappers. The
 * scaffold ships with values off by a factor of 1000 or with
 * the wrong unit; tests detect that.
 */

namespace task18
{

using namespace std::chrono_literals;

/* TODO: change to 50ms.
 * Stub mis-spells as 50us (50 microseconds, off by 1000). */
constexpr auto fifty_ms = 50us;

/* TODO: change to 2s.
 * Stub uses 2ms. */
constexpr auto two_seconds = 2ms;

/* TODO: change to 1min.
 * Stub uses 1s. */
constexpr auto one_minute = 1s;

/* TODO: change to 24h (one day).
 * Stub uses 1h. */
constexpr auto one_day = 1h;

/* TODO: change to 500ns.
 * Stub uses 500ms. */
constexpr auto five_hundred_ns = 500ms;

/*
 * A function returning a 100ms duration. Used by the tests
 * that demonstrate the UDL-deduced type matches
 * std::chrono::milliseconds.
 */
constexpr auto hundred_ms()
{
    /* TODO: return 100ms. */
    return 100s;
}

}
