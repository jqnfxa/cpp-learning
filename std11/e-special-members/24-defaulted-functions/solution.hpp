#pragma once

/*
 * Step 24 -- = default. See task.md for the spec.
 *
 * Declare task24::widget with:
 *   widget(int id);                       -- user-provided
 *   widget() = default;                   -- explicit default ctor
 *   widget(const widget &) = default;     -- explicit copy ctor
 *   widget &operator=(const widget &) = default;  -- explicit copy assign
 *   ~widget() = default;                  -- explicit dtor
 *
 * Implement widget(int) and id() with real bodies. Do not change
 * the declared signatures.
 */

namespace task24
{

class widget
{
public:
    /* TODO: implement (sets id_ = id). */
    widget(int /* id */)
    {
    }

    /* TODO: explicit default ctor with = default. */
    /* widget() = default; */

    /* TODO: explicit copy ctor with = default. */
    /* widget(const widget &) = default; */

    /* TODO: explicit copy assignment with = default. */
    /* widget &operator=(const widget &) = default; */

    /* TODO: explicit dtor with = default. */
    /* ~widget() = default; */

    /* TODO: return id_. */
    int id() const
    {
        return -1;
    }

private:
    int id_ = 0;
};

}
