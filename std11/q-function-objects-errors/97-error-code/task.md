# 97 — `std::error_code` / `error_condition` / `std::system_error`

> Spec: <https://en.cppreference.com/cpp/error/error_code>,
> <https://en.cppreference.com/cpp/error/system_error>

## What you learn

C++11 added a structured way to talk about *system-level
errors* — the kind that come from `errno`, OS calls, network
APIs, and so on. Three pieces:

| Type                   | Role                                          |
|------------------------|-----------------------------------------------|
| `std::error_code`      | Concrete integer + category (e.g. `2` + `generic_category`). |
| `std::error_condition` | Portable, platform-independent semantic equivalent. |
| `std::system_error`    | Exception type that wraps an `error_code` + a message. |

The headline use is the **errno → exception** bridge:

```cpp
#include <cstdio>
#include <system_error>
#include <cerrno>

FILE *f = std::fopen(path.c_str(), "r");
if (!f)
{
    throw std::system_error(errno, std::generic_category(),
                            "open failed: " + path);
}
```

When `fopen` fails, `errno` is set to a POSIX error code like
`ENOENT` (2, "No such file or directory"). The thrown exception
preserves that integer and lets the catch site recover the
machine-readable code *and* a human-readable message:

```cpp
catch (const std::system_error &e)
{
    int code = e.code().value();          /* e.g. ENOENT (2) */
    const std::error_category &cat
        = e.code().category();             /* generic_category() */
    const char *msg = e.what();           /* "open failed: ...: No such file or directory" */
}
```

### `error_code` vs `error_condition`

This is the part most people skip and later regret:

- **`error_code`**: a specific platform error -- e.g. POSIX
  `ENOENT`, or Windows `ERROR_FILE_NOT_FOUND`. Each lives in a
  different category. They are not equal across platforms.
- **`error_condition`**: a portable equivalence class --
  "no-such-file-or-directory" in the abstract. Both POSIX
  `ENOENT` and Windows `ERROR_FILE_NOT_FOUND` *map to* the same
  `std::errc::no_such_file_or_directory` condition.

Compare with `==` against `std::errc::...`:

```cpp
if (e.code() == std::errc::no_such_file_or_directory) { ... }
```

The comparison goes through the category's
`default_error_condition()` to do the right thing across
platforms. You write `errc::...` once and it works on POSIX *and*
Windows.

### Categories

- `std::generic_category()` -- the POSIX `errno` values (matches
  `<cerrno>`).
- `std::system_category()` -- the platform-native error space
  (on Linux/macOS, same as `generic_category`; on Windows, the
  Win32 error code space).

For `errno` errors from C library calls (`fopen`, `mkdir`,
`read`, etc.), use `std::generic_category()`. For raw OS
syscall errors, use `std::system_category()` -- on POSIX
they're identical in practice; on Windows they differ.

### Why not just `std::runtime_error`?

`runtime_error` carries a string. `system_error` carries a
string *and* a structured integer code plus category. Code that
needs to *react* to specific errors (e.g. "if EEXIST then
fallback to update path") can do so without parsing strings.

It's the difference between an exception that says "something
went wrong" and one that says "ENOENT happened, here's what
that means in this category". The structured form lets you
program against specific failure modes.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task97
{
    /* Open `path` for reading with std::fopen. On failure, throw
     * std::system_error built from errno + std::generic_category().
     * The exception's message must start with "open failed: " followed
     * by the path. */
    FILE *open_for_read(const std::string &path);

    /* Close the FILE*. No-op if f == nullptr. */
    void close_file(FILE *f);
}
```

`open_for_read` is the errno-to-exception bridge. The test for
missing files verifies that `.code().value() == ENOENT`.

## Run

```sh
./verify.sh
```

## Hints

- The translation is straightforward:
  ```cpp
  FILE *f = std::fopen(path.c_str(), "r");
  if (!f)
  {
      throw std::system_error(errno, std::generic_category(),
                              "open failed: " + path);
  }
  return f;
  ```

- `errno` is the right value *immediately after* the failing
  call. Capture it before you do anything else (especially
  before string concatenation or logging that might itself
  reset `errno`).

- `std::generic_category()` is the right choice for `errno`.
  `std::system_category()` would also work on POSIX but is
  platform-specific in general.

- For the close function, `std::fclose(f)` is enough. The
  `if (f)` guard prevents UB on null pointers (some C
  implementations are picky about `fclose(nullptr)`).

- Tests use `::testing::TempDir()` for the success-path file
  (we create a tiny file there, open it, close it). The
  failure-path uses a deterministic path like
  `/this/path/does/not/exist/x` to guarantee ENOENT.

- For the missing-file case, the test asserts
  `e.code().value() == ENOENT` (from `<cerrno>`). `ENOENT` is
  the POSIX "no such file or directory" code, typically 2 but
  the standard doesn't pin the integer.

- `e.code() == std::errc::no_such_file_or_directory` is the
  *portable* way to spell the same check. Both work in the
  test suite; we use the integer form to make the
  error_code-category mechanism visible.

- The `e.what()` string format is implementation-defined past
  the prefix you control. The test checks for the prefix
  substring, not exact equality.

- C++17 `<filesystem>` exceptions also embed an `error_code`.
  The same comparison idiom (`e.code() == std::errc::...`)
  applies. Knowing this protocol means you can react to
  errors uniformly across `<system_error>`, `<filesystem>`,
  network libraries (Boost.Asio), and database drivers --
  all built on the same `error_code` machinery.
