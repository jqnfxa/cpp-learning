#pragma once

#include <regex>
#include <string>

/*
 * Step 125 -- std::regex basics. See task.md for the spec.
 *
 * Validate an email-shaped string. The pattern is the common
 * local@domain.tld shape, not RFC-5322-complete.
 */

namespace task125
{

/* TODO:
 *   static const std::regex re(R"([A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,})");
 *   return std::regex_match(s, re);
 */
inline bool is_email_shaped(const std::string & /*s*/)
{
    return false;
}

}
