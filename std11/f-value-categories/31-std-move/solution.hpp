#pragma once

#include <utility>

/*
 * Step 31 -- std::move + swap. See task.md for the spec.
 *
 * Implement task31::swap as a function template that uses std::move
 * three times: T tmp = move(a); a = move(b); b = move(tmp);
 *
 * Do not change the signature.
 */

namespace task31
{

template <class T>
void swap(T & /* a */, T & /* b */)
{
    /* TODO: three-line swap using std::move. */
}

}
