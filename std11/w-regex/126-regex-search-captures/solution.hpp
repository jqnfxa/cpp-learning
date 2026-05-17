#pragma once

#include <regex>
#include <string>

/*
 * Step 126 -- regex_search + capture groups. See task.md for the spec.
 *
 * Parse "host:port" into a string and an int via two capture groups.
 */

namespace task126
{

/* TODO:
 *   static const std::regex re(R"(([^:]+):(\d+))");
 *   std::smatch m;
 *   if (!std::regex_match(s, m, re)) return false;
 *   host = m[1].str();
 *   port = std::stoi(m[2].str());
 *   return true;
 */
inline bool parse_host_port(const std::string & /*s*/, std::string & /*host*/,
                            int & /*port*/)
{
    return false;
}

}
