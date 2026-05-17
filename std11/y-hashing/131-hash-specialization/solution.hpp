#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <unordered_set>

/*
 * Step 131 -- std::hash specialization. See task.md for the spec.
 *
 * `point` is a small value type with two int fields. To use it in
 * std::unordered_set, std::hash<point> must be specialized AND
 * operator== must be defined. The scaffold ships with a BAD hash
 * that returns 0 for everything -- correct but pathological,
 * everything collides. The tests detect that.
 */

namespace task131
{

struct point
{
    int x;
    int y;
};

inline bool operator==(const point &a, const point &b) noexcept
{
    return a.x == b.x && a.y == b.y;
}

}

namespace std
{

template <>
struct hash<task131::point>
{
    /* TODO: combine x and y. Example (Boost's hash_combine):
     *   std::size_t h = std::hash<int>{}(p.x);
     *   h ^= std::hash<int>{}(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
     *   return h;
     */
    std::size_t operator()(const task131::point & /*p*/) const noexcept
    {
        return 0;
    }
};

}

namespace task131
{

using point_set = std::unordered_set<point>;

}
