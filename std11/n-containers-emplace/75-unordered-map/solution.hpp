#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

/*
 * Step 75 -- std::unordered_map<K, V>. See task.md for the spec.
 *
 * Implement word_count via ++m[w] and count_of via find().
 */

namespace task75
{

using counts = std::unordered_map<std::string, std::size_t>;

/* TODO: range-for over words and `++result[word]`. */
inline counts word_count(const std::vector<std::string> & /*words*/)
{
    return {};
}

/* TODO: m.find(word); if found return it->second else 0. */
inline std::size_t count_of(const counts & /*m*/, const std::string & /*word*/)
{
    return 0;
}

}
