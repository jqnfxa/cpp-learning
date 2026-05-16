#pragma once

/*
 * Step 41 -- user-defined literals: integer. See task.md.
 *
 * Implement two integer UDLs (`_km` and `_h`) that convert to
 * a "meters" or "seconds" long long. Each is one line.
 */

namespace task41
{

/* TODO: constexpr long long operator"" _km(unsigned long long km) {
 *   return static_cast<long long>(km * 1000);
 * } */
constexpr long long operator"" _km(unsigned long long /* km */)
{
    return 0;
}

/* TODO: constexpr long long operator"" _h(unsigned long long hours) {
 *   return static_cast<long long>(hours * 3600);
 * } */
constexpr long long operator"" _h(unsigned long long /* hours */)
{
    return 0;
}

}
