# 68 — `std::unique_ptr` with a custom deleter

> Spec: <https://en.cppreference.com/cpp/memory/unique_ptr>

## What you learn

`std::unique_ptr` is a *two-parameter* template:

```cpp
template <class T, class Deleter = std::default_delete<T>>
class unique_ptr;
```

The deleter parameter is what gets called on the held pointer in
the destructor. The default (`std::default_delete<T>`) just runs
`delete p`. If your resource isn't owned by `delete`, you supply
a different deleter:

- A C `FILE *` is closed with `std::fclose`.
- A POSIX socket is closed with `::close`.
- A SQLite handle is freed with `sqlite3_close`.
- An `IUnknown *` (COM) is released with `p->Release()`.

The pattern that lets `unique_ptr` manage *any* of these is the
same: pick a deleter type, point it at the right cleanup
function. The two common spellings:

```cpp
/* 1. Function-pointer deleter, captured via decltype. */
using c_file = std::unique_ptr<FILE, decltype(&std::fclose)>;
c_file f(std::fopen("data.txt", "r"), &std::fclose);

/* 2. Stateless lambda or struct deleter (zero size). */
struct file_closer
{
    void operator()(FILE *p) const noexcept { if (p) std::fclose(p); }
};
using c_file = std::unique_ptr<FILE, file_closer>;
c_file f(std::fopen("data.txt", "r"));   /* no deleter arg needed */
```

The decltype-function-pointer form is what almost all real
codebases use because it's the shortest spelling that adapts any
existing C cleanup function. The trade-off: the `unique_ptr` now
carries a pointer-sized deleter (`sizeof(unique_ptr) == 2 *
sizeof(void *)`). With the struct-deleter form, the deleter is
stateless and gets compressed (EBO) to zero — back to a single
pointer.

### Why "if (p)" in the deleter?

Standard C library functions like `std::fclose` are *not*
required to be safe on `nullptr`. Calling `fclose(nullptr)` is
undefined. `unique_ptr` guarantees it never *calls* the deleter
when the held pointer is null... when you use the *default*
deleter. With a custom deleter, the call happens regardless, so
the deleter must handle null itself.

Wait — actually `std::unique_ptr` *does* skip the call when the
pointer is null. So in this task you don't need the null check
in the deleter. But it's still a useful defensive habit, because
moved-from temporaries inside the deleter call hierarchy can
surprise you. For *this* task we rely on `unique_ptr`'s
guarantee.

### Returning from a function

A `unique_ptr<FILE, decltype(&std::fclose)>` returns and moves
exactly like the default form. The only awkward bit is
**constructing** it at the call site — the deleter is not
default-constructible (you must pass `&std::fclose`).

## Task

In `solution.hpp`, build a small RAII file wrapper:

```cpp
namespace task68
{
    using file_handle = std::unique_ptr<FILE, decltype(&std::fclose)>;

    /* fopen for write ("w"); the returned handle may be empty if the
     * open failed. */
    file_handle open_write(const char *path);

    /* fopen for read ("r"); empty handle on failure. */
    file_handle open_read(const char *path);

    /* fputs(line + '\n'). Throws std::runtime_error if the handle is
     * empty. */
    void write_line(const file_handle &h, const std::string &line);

    /* Slurp the whole file into a std::string. Throws on empty handle. */
    std::string read_all(const file_handle &h);
}
```

The point of the task is the *deleter spelling*:

```cpp
file_handle open_write(const char *path)
{
    return file_handle(std::fopen(path, "w"), &std::fclose);
}
```

That second argument is the only thing this task is really
about. Everything else is housekeeping so we have something to
test.

## Run

```sh
./verify.sh
```

## Hints

- `decltype(&std::fclose)` is `int(*)(FILE *)`. Both spellings
  work; the decltype form is preferred because it tracks the
  signature without you restating it.

- A `unique_ptr<FILE, decltype(&std::fclose)>` with a *null*
  pointer still requires a valid deleter argument at
  construction. You can't write `file_handle{}` — that calls
  `unique_ptr()` which requires a default-constructible deleter,
  and function pointers are not (more precisely: a null function
  pointer is constructible, but you'd then call through it on
  destruction if the held pointer were ever non-null). The
  canonical empty handle is `file_handle(nullptr, &std::fclose)`.

- For tests, `::testing::TempDir()` from gtest gives you a
  writable directory. Combine with a per-test filename to avoid
  collisions.

- The `if (p) std::fclose(p);` guard inside a *manual* deleter
  exists in real code because not all C libraries are safe on
  null — but `unique_ptr` itself never calls the deleter on a
  null pointer, so for this task it's redundant.

- The deleter being part of the *type* is what makes this
  `unique_ptr` instantiation distinct from
  `unique_ptr<FILE, some_other_deleter>`. They are not
  interconvertible without an explicit move and a deleter
  conversion. This is intentional — it's how the type system
  catches mixing different cleanup strategies.

- `std::shared_ptr` takes the deleter at *construction* (not as
  a template parameter) and type-erases it. That's why
  `shared_ptr<FILE>` for the same use case doesn't need any
  `decltype` ceremony. The trade-off is a heavier control block
  and an indirect dispatch on destruction.
