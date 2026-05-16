#pragma once

#include <cstddef>

/*
 * Step 07 -- alignas. See task.md for the spec.
 *
 * Fix the cache_line struct below so:
 *   - alignof(cache_line) == 64
 *   - sizeof(cache_line)  == 64
 *   - it contains a single char[64] member named `data`
 *
 * The get_instance() function is already implemented and does not
 * need changes.
 */

namespace task07
{

/* TODO: add alignas(64) and grow `data` to 64 bytes. */
struct cache_line
{
    char data[1];
};

inline const cache_line *get_instance()
{
    static cache_line instance{};
    return &instance;
}

}
