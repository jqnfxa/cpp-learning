#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

/*
 * Step 22 -- two-range overloads of std::equal, std::mismatch,
 * std::is_permutation (C++14). See task.md.
 *
 * C++11 versions take (first1, last1, first2) -- a one-and-a-
 * half-range form. The second range is assumed to be at least
 * as long as the first; if it's shorter, you read past the
 * end (UB).
 *
 * C++14 added (first1, last1, first2, last2) overloads. They
 * check both endpoints, returning false / a mismatch as soon
 * as either range runs out.
 *
 * Three demos: safe_equal, safe_mismatch_size, safe_perm.
 *
 * The scaffold ships with the C++11 three-iterator form. For
 * same-length inputs the answers agree; for different-length
 * inputs the stub reads past the end of the second range.
 * Tests with intentionally-different-sized inputs catch it.
 */

namespace task22
{

/*
 * Return true iff [b1, e1) and [b2, e2) have the SAME LENGTH
 * and equal elements.
 *
 * The C++14 four-argument std::equal does exactly this; the
 * C++11 three-arg form ignores [b2, e2)'s length and may
 * return true for a-prefix-of-b cases.
 */
template <class It1, class It2>
bool safe_equal(It1 b1, It1 e1, It2 b2, It2 e2)
{
    /* TODO: return std::equal(b1, e1, b2, e2); */
    (void)e2;
    return std::equal(b1, e1, b2);
}

/*
 * Return true iff the two ranges have the same length and
 * the first mismatch is at the END (i.e., no mismatch).
 *
 * Uses std::mismatch's C++14 four-argument form which stops
 * at the shorter range.
 */
template <class It1, class It2>
bool both_ranges_match_completely(It1 b1, It1 e1, It2 b2, It2 e2)
{
    /* TODO: auto p = std::mismatch(b1, e1, b2, e2);
     *       return p.first == e1 && p.second == e2;
     */
    auto p = std::mismatch(b1, e1, b2);
    return p.first == e1;
}

/*
 * Return true iff [b1, e1) is a permutation of [b2, e2).
 * Uses the C++14 four-argument std::is_permutation, which
 * first verifies the lengths match.
 *
 * The C++11 three-arg form assumes the lengths match and
 * doesn't check. Passing ranges of different sizes is UB.
 */
template <class It1, class It2>
bool safe_perm(It1 b1, It1 e1, It2 b2, It2 e2)
{
    /* TODO: return std::is_permutation(b1, e1, b2, e2); */
    (void)e2;
    return std::is_permutation(b1, e1, b2);
}

}
