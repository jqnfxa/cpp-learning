#pragma once

#include <cstddef>
#include <memory>
#include <new>
#include <string>
#include <vector>

/*
 * Step 91 -- std::uninitialized_copy_n. See task.md for the spec.
 *
 * Implement the full uninitialized-storage round-trip:
 *   raw alloc -> placement-construct via uninitialized_copy_n ->
 *   build output vector -> explicit destroy -> raw dealloc.
 */

namespace task91
{

/* TODO:
 *   void *raw = ::operator new(count * sizeof(std::string));
 *   auto *p = static_cast<std::string *>(raw);
 *   std::uninitialized_copy_n(src.begin(), count, p);
 *   std::vector<std::string> out(p, p + count);
 *   for (std::size_t i = 0; i < count; ++i) p[i].~basic_string();
 *   ::operator delete(raw);
 *   return out;
 */
inline std::vector<std::string>
via_uninitialized_copy_n(const std::vector<std::string> & /*src*/, std::size_t /*count*/)
{
    return {};
}

}
