#pragma once

#include <optional>
#include <string>

/*
 * Step 29 -- optional operations: value_or, has_value, *, ->.
 */

namespace task29
{

inline std::optional<int> parse_int(const std::string &s)
{
    try
    {
        size_t pos = 0;
        int v = std::stoi(s, &pos);
        if (pos != s.size())
        {
            return std::nullopt;
        }
        return v;
    }
    catch (...)
    {
        return std::nullopt;
    }
}

/* TODO: return parse_int(s).value_or(default_value); */
/* TODO: return parse_int(s).value_or(default_value); */
inline int parse_int_or(const std::string &s, int default_value)
{
    (void)s;
    return default_value;
}

/* TODO:
 *   auto v = parse_int(s);
 *   if (v) return *v * 2;
 *   return 0;
 */
/* TODO:
 *   auto v = parse_int(s);
 *   if (v) return *v * 2;
 *   return 0;
 */
inline int parse_int_doubled(const std::string &s)
{
    (void)s;
    return 0;
}

}
