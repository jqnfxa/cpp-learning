#pragma once

/*
 * Step 7 -- variable templates (C++14). See task.md.
 *
 * `template <class T> constexpr T NAME = INIT;` defines a
 * variable whose type and initialiser are parameterised by T.
 * Each instantiation (`NAME<float>`, `NAME<double>`, ...)
 * yields a distinct constexpr value with the right type.
 *
 * The scaffold ships with `T(0)` initialisers -- the variables
 * exist (so type-checking tests pass) but the VALUES are
 * zero. Tests detect that.
 */

namespace task07
{

/* TODO: change `T(0)` to `T(3.14159265358979323846L)` so the
 * value matches pi at long-double precision (the suffix L makes
 * the literal long double, then T(...) narrows as needed). */
template <class T>
constexpr T pi = T(0);

/* TODO: change `T(0)` to `T(2.71828182845904523536L)`. */
template <class T>
constexpr T e = T(0);

}
