#pragma once

/*
 * Step 53 -- captureless lambda -> function pointer. See task.md.
 *
 * Two functions:
 *   get_doubler() -- returns int(*)(int), implemented as a
 *                    captureless lambda implicitly converted.
 *   apply(fn, x)  -- calls fn(x) and returns the result.
 */

namespace task53
{

/* TODO: return [](int x) { return x * 2; }; */
inline int (*get_doubler())(int)
{
    return nullptr;
}

/* TODO: return fn(x); */
inline int apply(int (* /* fn */)(int), int /* x */)
{
    return 0;
}

}
