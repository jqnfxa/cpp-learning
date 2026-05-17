#pragma once

#include <regex>
#include <string>
#include <vector>

/*
 * Step 127 -- std::sregex_iterator. See task.md for the spec.
 *
 * Walk every match of `\w+` and collect the substrings as words.
 */

namespace task127
{

/* TODO:
 *   static const std::regex word(R"(\w+)");
 *   std::vector<std::string> out;
 *   for (auto it = std::sregex_iterator(s.begin(), s.end(), word);
 *        it != std::sregex_iterator(); ++it)
 *   {
 *       out.push_back(it->str());
 *   }
 *   return out;
 */
inline std::vector<std::string> tokenize_words(const std::string & /*s*/)
{
    return {};
}

}
