#pragma once

#include <cstddef>
#include <vector>

/*
 * Step 43 -- user-defined literals: string. See task.md.
 *
 * Two string UDLs. Cooked form takes (const char *, std::size_t).
 */

namespace task43
{

/* TODO: return std::vector<unsigned char>(str, str + len); */
inline std::vector<unsigned char> operator"" _bytes(const char * /* str */, std::size_t /* len */)
{
    return {};
}

/* TODO: return len; */
inline std::size_t operator"" _len(const char * /* str */, std::size_t /* len */)
{
    return 0;
}

}
