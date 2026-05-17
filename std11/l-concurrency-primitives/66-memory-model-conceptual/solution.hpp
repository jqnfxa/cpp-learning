#pragma once

/*
 * Step 66 -- C++11 memory model (conceptual). See task.md for the spec.
 *
 * Set each constant to `true` if the described scenario is a data race
 * under the C++11 memory model, `false` otherwise. The tests are
 * static_asserts; wrong answers refuse to compile.
 */

namespace task66
{

/* Two threads both only do `(void)shared;` -- two non-atomic reads. */
/* TODO: set the correct value. */
constexpr bool scenario_1_both_read_plain = true;

/* Thread A writes `shared = 1;`, thread B reads `int x = shared;`.
 * Plain `int`, no synchronisation. */
/* TODO: set the correct value. */
constexpr bool scenario_2_write_then_read = false;

/* Thread A writes `a = 1;`, thread B writes `b = 2;` -- two different
 * plain ints, no synchronisation. */
/* TODO: set the correct value. */
constexpr bool scenario_3_disjoint_writes = true;

/* Thread A does `shared.store(1)`, thread B does `shared.load()` --
 * `shared` is `std::atomic<int>`. */
/* TODO: set the correct value. */
constexpr bool scenario_4_atomic_pair = true;

/* Both threads lock `m`, then read/write `shared`, then unlock. */
/* TODO: set the correct value. */
constexpr bool scenario_5_mutex_protected = true;

/* Two threads both do `++shared;` on a plain `int`, no sync. */
/* TODO: set the correct value. */
constexpr bool scenario_6_concurrent_writes = false;

}
