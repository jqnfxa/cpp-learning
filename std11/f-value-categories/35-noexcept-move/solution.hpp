#pragma once

/*
 * Step 35 -- noexcept move ops. See task.md for the spec.
 *
 * Declare two classes inside task35 differing only in whether
 * their move constructor is noexcept:
 *
 *   safe_mover    -- move ctor is noexcept
 *   unsafe_mover  -- move ctor is NOT noexcept
 *
 * Bodies can be empty; the test inspects type traits, not behavior.
 */

namespace task35
{

class safe_mover
{
public:
    safe_mover() = default;
    safe_mover(const safe_mover & /* other */)
    {
    }

    /* TODO: declare and define the move ctor as noexcept. */
};

class unsafe_mover
{
public:
    unsafe_mover() = default;
    unsafe_mover(const unsafe_mover & /* other */)
    {
    }

    /* TODO: declare and define the move ctor WITHOUT noexcept. */
};

}
