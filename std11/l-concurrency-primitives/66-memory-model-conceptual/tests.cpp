#include <gtest/gtest.h>

#include "solution.hpp"

/* Six classifications. Each correct answer is pinned by a static_assert.
 * If the student's answer disagrees with the C++11 memory model, the
 * build fails -- which is the right failure mode for a pure
 * classification task. */

static_assert(task66::scenario_1_both_read_plain == false,
              "two concurrent non-atomic reads of the same int are NOT a data race "
              "(rule: at least one access must be a write)");

static_assert(task66::scenario_2_write_then_read == true,
              "one writer + one reader on a plain int with no synchronisation IS a race");

static_assert(task66::scenario_3_disjoint_writes == false,
              "writes to different memory locations are NOT a race "
              "(rule: same memory location)");

static_assert(task66::scenario_4_atomic_pair == false,
              "store + load on the same std::atomic<int> are NOT a race "
              "(atomics provide the synchronisation)");

static_assert(task66::scenario_5_mutex_protected == false,
              "accesses serialised by the same std::mutex are NOT a race "
              "(mutex creates happens-before)");

static_assert(task66::scenario_6_concurrent_writes == true,
              "two concurrent writes to the same plain int with no synchronisation IS a race");

TEST(memory_model, classifications)
{
    /* The classification is pure metaprogramming; this test exists just
     * so there is a runtime artifact to run. The static_asserts above
     * are the real verification. */
    SUCCEED();
}
