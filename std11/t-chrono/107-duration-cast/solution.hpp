#pragma once

#include <chrono>

/*
 * Step 107 -- duration arithmetic and std::chrono::duration_cast.
 * See task.md for the spec.
 *
 * Three conversion / division helpers.
 */

namespace task107
{

/* TODO: return std::chrono::duration_cast<std::chrono::seconds>(ms); */
inline std::chrono::seconds to_seconds(std::chrono::milliseconds /*ms*/)
{
    return std::chrono::seconds(0);
}

/* TODO: return s; (implicit lossless conversion) */
inline std::chrono::milliseconds to_milliseconds(std::chrono::seconds /*s*/)
{
    return std::chrono::milliseconds(0);
}

/* TODO: return total / chunk; */
inline long long divide(std::chrono::milliseconds /*total*/,
                        std::chrono::milliseconds /*chunk*/)
{
    return 0;
}

}
