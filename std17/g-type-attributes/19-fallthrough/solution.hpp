#pragma once

#include <string>

/*
 * Step 19 -- [[fallthrough]] (C++17). See task.md.
 *
 * The attribute marks an intentional case-to-case fall-through
 * in a switch, suppressing compiler warnings.
 *
 * Reference: case 0 falls through to case 1/2; case 1 and 2
 * append "small"; case 3 returns "three".
 */

namespace task19
{

/*
 * TODO:
 *   std::string result;
 *   switch (x) {
 *     case 0:
 *       result += "zero ";
 *       [[fallthrough]];
 *     case 1:
 *     case 2:
 *       result += "small";
 *       break;
 *     case 3:
 *       result += "three";
 *       break;
 *     default:
 *       result += "other";
 *   }
 *   return result;
 */
inline std::string classify(int x)
{
    /* TODO: implement the switch with [[fallthrough]] -- see
     * the top-of-file comment. */
    (void)x;
    return "";
}

}
