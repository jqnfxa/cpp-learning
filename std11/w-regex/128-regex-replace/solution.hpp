#pragma once

#include <regex>
#include <string>

/*
 * Step 128 -- std::regex_replace. See task.md for the spec.
 *
 * Redact every digit in the input with '#'. Stub returns the
 * input unchanged.
 */

namespace task128
{

/* TODO:
 *   static const std::regex digits(R"(\d)");
 *   return std::regex_replace(s, digits, "#");
 */
inline std::string redact_digits(const std::string &s)
{
    return s;
}

}
