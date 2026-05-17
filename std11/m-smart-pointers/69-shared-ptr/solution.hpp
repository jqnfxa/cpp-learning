#pragma once

#include <memory>

/*
 * Step 69 -- std::shared_ptr<T>. See task.md for the spec.
 *
 * Implement an API that shares ownership between two functions
 * (caller and stash) and exposes use_count() so tests can pin
 * the count transitions.
 */

namespace task69
{

/* TODO: return std::make_shared<int>(n). */
inline std::shared_ptr<int> make_value(int /*n*/)
{
    return std::shared_ptr<int>();
}

namespace detail
{
inline std::shared_ptr<int> &slot()
{
    static std::shared_ptr<int> s;
    return s;
}
}

/* TODO: assign p into the static slot; return the slot's use_count(). */
inline long stash(const std::shared_ptr<int> & /*p*/)
{
    return 0;
}

/* TODO: if the slot is non-empty, reset it and return true; else false. */
inline bool release()
{
    return false;
}

/* TODO: return a copy of the static slot. */
inline std::shared_ptr<int> stashed()
{
    return std::shared_ptr<int>();
}

}
