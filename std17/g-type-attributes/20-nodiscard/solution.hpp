#pragma once

/*
 * Step 20 -- [[nodiscard]] (C++17). See task.md.
 *
 * `[[nodiscard]]` on a function or type warns when the return
 * value is discarded. Useful for functions whose return is
 * the only meaningful effect (error codes, computed results).
 *
 * The reference's annotations don't change runtime behavior --
 * they only trigger compiler warnings at use sites. The
 * functional tests check the return values.
 */

namespace task20
{

/* TODO: add [[nodiscard]] before the return type, and
 * return x * 2 (the stub returns just x). */
inline int must_read(int x)
{
    return x + 0;
}

/* [[nodiscard]] on a type means ALL functions returning it
 * are nodiscard. */
struct [[nodiscard]] error_code
{
    int value;
};

inline error_code parse(int x)
{
    /* TODO: return error_code{x * 3}; */
    return error_code{0};
}

}
