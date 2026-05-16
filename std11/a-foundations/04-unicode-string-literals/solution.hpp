#pragma once

/*
 * Step 04 -- Unicode string literals. See task.md for the spec.
 *
 * Implement four functions returning string literals in different
 * encodings. The return type forces the prefix you must use for
 * UTF-16 and UTF-32. For UTF-8 and the raw case, pick the prefix
 * that makes the bytes correct and the source readable.
 *
 * Do not change the function signatures.
 */

namespace task04
{

inline const char *as_utf8()
{
    return "héllo";
}

inline const char16_t *as_utf16()
{
    /* TODO: return the UTF-16 encoding of "héllo" using a u literal */
    return u"";
}

inline const char32_t *as_utf32()
{
    /* TODO: return the UTF-32 encoding of "héllo" using a U literal */
    return U"";
}

inline const char *as_raw()
{
    /* TODO: return the 8-char string \d+\.\d+ using a raw literal */
    return "";
}

}
