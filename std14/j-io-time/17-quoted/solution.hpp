#pragma once

#include <iomanip>
#include <sstream>
#include <string>

/*
 * Step 17 -- std::quoted (C++14). See task.md.
 *
 * std::quoted is a stream manipulator that wraps a string in
 * double quotes on output and reads back a quoted string on
 * input. Embedded quotes are escaped with backslash.
 *
 * Two functions:
 *   - serialize: turn a string into its quoted form.
 *   - round_trip: write quoted, then read back. Should return
 *     the original string (including spaces and embedded
 *     quotes).
 *
 * The scaffold ships with stubs that don't use std::quoted at
 * all -- they truncate at the first whitespace, losing the
 * "preserves embedded spaces" property.
 */

namespace task17
{

/*
 * Return the quoted serialised form of s.
 *
 * For s = `hello world`, return `"hello world"` (including
 * the double quotes).
 *
 * For s = `say "hi"`, return `"say \"hi\""` (embedded quotes
 * escaped).
 */
inline std::string serialize(const std::string &s)
{
    /* TODO:
     *   std::ostringstream oss;
     *   oss << std::quoted(s);
     *   return oss.str();
     *
     * The stub just returns s without any quoting/escaping. */
    return s;
}

/*
 * Write s through std::quoted, read it back through std::quoted,
 * return the deserialised value. Should equal s exactly.
 *
 * The interesting cases are strings containing spaces (which
 * default `>>` would split on) and embedded quotes (which
 * std::quoted escapes).
 */
inline std::string round_trip(const std::string &s)
{
    /* TODO:
     *   std::stringstream ss;
     *   ss << std::quoted(s);
     *   std::string out;
     *   ss >> std::quoted(out);
     *   return out;
     *
     * The stub uses naive << / >> which splits at whitespace. */
    std::stringstream ss;
    ss << s;
    std::string out;
    ss >> out;
    return out;
}

}
