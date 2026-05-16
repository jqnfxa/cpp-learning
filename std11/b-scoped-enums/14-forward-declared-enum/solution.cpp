#include "solution.hpp"

/*
 * Step 14 -- forward-declared enums. See task.md for the spec.
 *
 * Provide:
 *   1. The full definition of `state` with enumerators idle=0,
 *      running=1, done=2.
 *   2. Bodies for make_idle, make_running, make_done.
 *   3. is_terminal(s) returning true iff s == state::done.
 *   4. name_of(s) returning "idle" / "running" / "done".
 *
 * Do not change the function signatures.
 */

namespace task14
{

/* TODO: replace this placeholder list with idle = 0, running = 1, done = 2. */
enum class state : std::uint8_t
{
    placeholder = 0
};

state make_idle()
{
    /* TODO: return state::idle */
    return state::placeholder;
}

state make_running()
{
    /* TODO: return state::running */
    return state::placeholder;
}

state make_done()
{
    /* TODO: return state::done */
    return state::placeholder;
}

bool is_terminal(state /* s */)
{
    /* TODO: return s == state::done */
    return false;
}

const char *name_of(state /* s */)
{
    /* TODO: switch on s, return "idle" / "running" / "done" */
    return "?";
}

}
