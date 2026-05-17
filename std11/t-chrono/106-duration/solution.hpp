#pragma once

#include <chrono>

/*
 * Step 106 -- std::chrono::duration. See task.md for the spec.
 *
 * Four small functions that exercise duration construction,
 * count(), and addition across units.
 */

namespace task106
{

/* TODO: return std::chrono::seconds(5); */
inline std::chrono::seconds five_seconds()
{
    return std::chrono::seconds(0);
}

/* TODO: return std::chrono::milliseconds(100); */
inline std::chrono::milliseconds hundred_milliseconds()
{
    return std::chrono::milliseconds(0);
}

/* TODO: return s.count(); */
inline long long count_of(std::chrono::seconds /*s*/)
{
    return 0;
}

/* TODO: return s + ms; */
inline std::chrono::milliseconds add(std::chrono::seconds /*s*/,
                                     std::chrono::milliseconds /*ms*/)
{
    return std::chrono::milliseconds(0);
}

}
