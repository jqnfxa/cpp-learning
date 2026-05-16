#pragma once

#include <utility>

/*
 * Step 36 -- std::forward + perfect forwarding. See task.md.
 *
 * Implement `factory<T, Args...>(Args &&... args)` that constructs
 * a T from the forwarded arguments. One line: return
 * T(std::forward<Args>(args)...);
 */

namespace task36
{

template <class T, class... Args>
T factory(Args &&.../* args */)
{
    /* TODO: return T(std::forward<Args>(args)...); */
    return T();
}

}
