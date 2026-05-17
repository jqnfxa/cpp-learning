#pragma once

#include <type_traits>

/*
 * Step 104 -- std::conditional. See task.md for the spec.
 *
 * Define wider<A, B>::type as the wider of the two by sizeof.
 * Tie-breaker: prefer A.
 */

namespace task104
{

/* TODO:
 *   using type = typename std::conditional<
 *       (sizeof(A) >= sizeof(B)), A, B>::type;
 */
template <class A, class /*B*/>
struct wider
{
    using type = A;
};

}
