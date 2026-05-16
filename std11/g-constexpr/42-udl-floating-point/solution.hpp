#pragma once

/*
 * Step 42 -- user-defined literals: floating-point. See task.md.
 *
 * Two long-double UDLs: _deg (degrees -> radians) and _C
 * (Celsius -> Kelvin).
 */

namespace task42
{

/* TODO: return deg * pi / 180. Use 3.14159265358979323846L as pi. */
constexpr long double operator"" _deg(long double /* deg */)
{
    return 0.0L;
}

/* TODO: return celsius + 273.15L. */
constexpr long double operator"" _C(long double /* celsius */)
{
    return 0.0L;
}

}
