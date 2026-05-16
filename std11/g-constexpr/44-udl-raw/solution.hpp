#pragma once

/*
 * Step 44 -- user-defined literals: raw form. See task.md.
 *
 * Implement a raw integer UDL `_bin` that parses a binary literal
 * given to it as `const char *`. Digit separator `'\''` is
 * allowed and ignored.
 */

namespace task44
{

/* TODO: helper -- recursive single-return parser.
 *   - *s == '\0' -> return acc
 *   - *s == '\'' -> recurse with s+1, acc
 *   - else        -> recurse with s+1, (acc << 1) | (*s - '0')
 */
constexpr unsigned long long parse_bin(const char * /* s */, unsigned long long /* acc */)
{
    return 0;
}

/* TODO: return parse_bin(str, 0); */
constexpr unsigned long long operator"" _bin(const char * /* str */)
{
    return 0;
}

}
