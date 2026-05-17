/*
 * helper.cpp -- second TU that touches the same `global_counter`.
 *
 * If `solution.hpp` declares `global_counter` as `static`, this
 * TU gets its OWN copy -- separate from tests.cpp. If declared
 * as `inline` (C++17), all TUs share the same single
 * definition.
 */

#include "solution.hpp"

namespace task14
{

int helper_increment_counter()
{
    return ++global_counter;
}

int helper_read_counter()
{
    return global_counter;
}

}
