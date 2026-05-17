#pragma once

/*
 * Step 45 -- noexcept specifier. See task.md for the spec.
 *
 * Declare and implement a widget class plus two free functions
 * with these noexcept profiles:
 *   widget::clone()    -- noexcept (true)
 *   widget::compute()  -- not noexcept (default)
 *   free_safe()        -- noexcept
 *   free_unsafe()      -- not noexcept
 *
 * Bodies can return anything; the *specifier* is the lesson.
 */

namespace task45
{

class widget
{
public:
    /* TODO: mark clone() noexcept. */
    int clone() const
    {
        return 0;
    }

    /* compute() must NOT be noexcept (default -- don't add the keyword). */
    int compute() const
    {
        return 0;
    }
};

/* TODO: mark free_safe() noexcept. */
inline int free_safe()
{
    return 0;
}

inline int free_unsafe()
{
    return 0;
}

}
