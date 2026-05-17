#pragma once

#include <cerrno>
#include <cstdio>
#include <string>
#include <system_error>

/*
 * Step 97 -- std::error_code / system_error. See task.md for the spec.
 *
 * Open a file via std::fopen; convert failure into a std::system_error
 * built from errno + std::generic_category().
 */

namespace task97
{

/* TODO:
 *   FILE *f = std::fopen(path.c_str(), "r");
 *   if (!f) throw std::system_error(errno, std::generic_category(),
 *                                   "open failed: " + path);
 *   return f;
 */
inline FILE *open_for_read(const std::string & /*path*/)
{
    return nullptr;
}

/* TODO: if (f) std::fclose(f); */
inline void close_file(FILE * /*f*/)
{
}

}
