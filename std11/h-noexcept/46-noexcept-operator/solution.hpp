#pragma once

/*
 * Step 46 -- noexcept operator. See task.md.
 *
 * Implement four constexpr bools that each query `noexcept(EXPR)`
 * for a specific expression. Each body is one line.
 */

namespace task46
{

void safe() noexcept;
void unsafe();

/* TODO: return noexcept(safe()); */
constexpr bool calling_safe_is_noexcept()
{
    return false;
}

/* TODO: return noexcept(unsafe()); */
constexpr bool calling_unsafe_is_noexcept()
{
    return true;
}

/* TODO: return noexcept(int{}); */
constexpr bool int_default_init_is_noexcept()
{
    return false;
}

/* TODO: return noexcept(new int(0)); */
constexpr bool new_int_is_noexcept()
{
    return true;
}

}
