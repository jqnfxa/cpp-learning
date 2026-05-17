#pragma once

/*
 * Step 64 -- trivial and standard-layout. See task.md for the spec.
 *
 * Declare four types so the static_asserts in tests.cpp pass:
 *
 *   trivial_pod         -- trivial AND standard-layout.
 *   trivial_not_layout  -- trivial but NOT standard-layout.
 *   layout_not_trivial  -- NOT trivial but standard-layout.
 *   neither             -- neither trivial nor standard-layout.
 */

namespace task64
{

/* TODO: two int members, defaulted everything. */
struct trivial_pod
{
    int a;
};

/* TODO: mixed-access data members break standard-layout while keeping
 * triviality. */
struct trivial_not_layout
{
    int a;
};

/* TODO: user-provided default constructor breaks triviality without
 * breaking standard-layout. */
struct layout_not_trivial
{
    int a;
};

/* TODO: a virtual function ruins both at once. */
struct neither
{
    int a;
};

}
