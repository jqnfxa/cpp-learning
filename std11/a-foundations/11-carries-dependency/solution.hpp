#pragma once

/*
 * Step 11 -- [[carries_dependency]] attribute. See task.md for the spec.
 *
 * 1. Add [[carries_dependency]] before the return type of get_token.
 * 2. Add [[carries_dependency]] before the parameter type of
 *    consume_token's `p`.
 * 3. Implement both bodies as described in task.md.
 *
 * The tests don't observe the attribute directly (no trait for it
 * exists). They only verify the function bodies behave correctly.
 * The attribute is for exposure to the syntax.
 */

namespace task11
{

/* TODO: add [[carries_dependency]] and return a pointer to a
 * function-local static const int with value 42. */
inline const int *get_token()
{
    return nullptr;
}

/* TODO: add [[carries_dependency]] to the parameter `p` and return
 * the dereferenced value. */
inline int consume_token(const int *p)
{
    (void)p;
    return 0;
}

}
