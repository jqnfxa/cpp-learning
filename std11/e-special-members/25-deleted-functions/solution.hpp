#pragma once

/*
 * Step 25 -- = delete. See task.md for the spec.
 *
 * Declare task25::unique_resource with three defaulted special
 * members and two deleted ones. Implement id() to return id_.
 */

namespace task25
{

class unique_resource
{
public:
    /* TODO: explicit defaults for default ctor, move ctor, move assign. */
    /* unique_resource() = default; */
    /* unique_resource(unique_resource &&) = default; */
    /* unique_resource &operator=(unique_resource &&) = default; */

    /* TODO: explicit deletes for copy ctor and copy assign. */
    /* unique_resource(const unique_resource &) = delete; */
    /* unique_resource &operator=(const unique_resource &) = delete; */

    /* TODO: return id_. */
    int id() const
    {
        return -1;
    }

private:
    int id_ = 7;
};

}
