#pragma once

#include <ratio>

/*
 * Step 110 -- std::ratio. See task.md for the spec.
 *
 * Define one_half as ratio_add<ratio<1,3>, ratio<1,6>>.
 */

namespace task110
{

/* TODO:
 *   using one_half = std::ratio_add<std::ratio<1, 3>, std::ratio<1, 6>>;
 */
using one_half = std::ratio<0, 1>;

}
