#pragma once

/*
 * Step 27 -- removed: trigraphs and `register` (C++17).
 *
 * - Trigraphs (`??=`, `??<`, `??>`, etc.) were removed.
 * - The `register` keyword is gone as a storage-class
 *   specifier (still reserved for future use).
 *
 * This is read-only -- no code change. The presence of the
 * file is documentation.
 */

namespace task27
{

/* Identity function -- placeholder. */
constexpr int passthrough(int x)
{
    return x;
}

}
