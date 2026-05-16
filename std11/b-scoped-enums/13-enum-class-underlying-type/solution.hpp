#pragma once

#include <cstdint>

/*
 * Step 13 -- enum class with explicit underlying type. See task.md.
 *
 * Declare three scoped enums:
 *   status : std::uint8_t   { ok = 0, fail = 1 }
 *   field  : std::uint16_t  { one, two, three }
 *   id     : std::uint64_t  { sentinel = static_cast<std::uint64_t>(-1) }
 *
 * The tests reference each by name, so missing declarations make
 * tests.cpp fail to compile.
 */

namespace task13
{

/* TODO: declare the three scoped enums listed above. */

}
