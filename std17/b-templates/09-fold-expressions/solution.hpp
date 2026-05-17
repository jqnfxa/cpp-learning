#pragma once

#include <sstream>
#include <string>

/*
 * Step 9 -- fold expressions (C++17). See task.md.
 *
 * Fold expressions reduce a parameter pack with an operator.
 * Four forms:
 *
 *   Unary  left  fold:  (... op pack)        == ((p1 op p2) op p3) op ...
 *   Unary  right fold:  (pack op ...)        == p1 op (p2 op (p3 op ...))
 *   Binary left  fold:  (init op ... op pack)
 *   Binary right fold:  (pack op ... op init)
 *
 * Four functions exercise the forms. The scaffold returns the
 * identity element (0, 1, true, "") -- tests detect.
 */

namespace task09
{

/* TODO: return (... + args); */
template <class... Args>
auto sum(Args... args)
{
    (void)sizeof...(args);
    return 0;
}

/* TODO: return (... * args); */
template <class... Args>
auto product(Args... args)
{
    (void)sizeof...(args);
    return 1;
}

/* TODO: return (... && args); */
template <class... Args>
bool all_true(Args... args)
{
    (void)sizeof...(args);
    return false;
}

/* TODO: write `((oss << args << " "), ...);` -- the unary
 * right fold over the comma operator. Streams each arg into
 * oss in order, with a space between. */
template <class... Args>
std::string concat_with_spaces(Args const &... args)
{
    std::ostringstream oss;
    (void)oss;
    (void)sizeof...(args);
    return "";
}

}
