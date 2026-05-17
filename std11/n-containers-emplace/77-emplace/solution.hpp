#pragma once

#include <deque>
#include <string>
#include <utility>
#include <vector>

/*
 * Step 77 -- emplace / emplace_back / emplace_front.
 *
 * The Tracker type counts value/copy/move constructor invocations so
 * tests can prove that emplace_* avoids the temporary that
 * push_back(T{...}) creates.
 *
 * Implement three one-line factories using each construction idiom.
 */

namespace task77
{

inline int &value_ctor_count()
{
    static int n = 0;
    return n;
}
inline int &copy_ctor_count()
{
    static int n = 0;
    return n;
}
inline int &move_ctor_count()
{
    static int n = 0;
    return n;
}

inline void reset_counters()
{
    value_ctor_count() = 0;
    copy_ctor_count() = 0;
    move_ctor_count() = 0;
}

struct Tracker
{
    int n;
    std::string name;

    Tracker(int nn, std::string nm)
        : n(nn), name(std::move(nm))
    {
        ++value_ctor_count();
    }

    Tracker(const Tracker &o)
        : n(o.n), name(o.name)
    {
        ++copy_ctor_count();
    }

    Tracker(Tracker &&o) noexcept
        : n(o.n), name(std::move(o.name))
    {
        ++move_ctor_count();
    }

    Tracker &operator=(const Tracker &) = default;
    Tracker &operator=(Tracker &&) = default;
    ~Tracker() = default;
};

/* TODO: v.push_back(Tracker(n, name));  -- one value ctor + one move ctor. */
inline std::vector<Tracker> via_push_back(int /*n*/, std::string /*name*/)
{
    return {};
}

/* TODO: v.emplace_back(n, name);  -- one value ctor, no moves. */
inline std::vector<Tracker> via_emplace_back(int /*n*/, std::string /*name*/)
{
    return {};
}

/* TODO: d.emplace_front(n, name);  -- one value ctor, no moves. */
inline std::deque<Tracker> via_emplace_front(int /*n*/, std::string /*name*/)
{
    return {};
}

}
