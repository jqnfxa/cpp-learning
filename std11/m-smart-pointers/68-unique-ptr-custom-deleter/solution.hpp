#pragma once

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

/*
 * Step 68 -- std::unique_ptr with a custom deleter. See task.md.
 *
 * Implement a small RAII wrapper for `FILE *` so the destructor runs
 * `std::fclose` automatically.
 */

namespace task68
{

using file_handle = std::unique_ptr<FILE, decltype(&std::fclose)>;

/* TODO: return file_handle(std::fopen(path, "w"), &std::fclose). */
inline file_handle open_write(const char * /*path*/)
{
    return file_handle(nullptr, &std::fclose);
}

/* TODO: return file_handle(std::fopen(path, "r"), &std::fclose). */
inline file_handle open_read(const char * /*path*/)
{
    return file_handle(nullptr, &std::fclose);
}

/* TODO: throw if !h; otherwise fputs the line and a '\n'. */
inline void write_line(const file_handle & /*h*/, const std::string & /*line*/)
{
}

/* TODO: throw if !h; otherwise slurp the whole file. */
inline std::string read_all(const file_handle & /*h*/)
{
    return {};
}

}
