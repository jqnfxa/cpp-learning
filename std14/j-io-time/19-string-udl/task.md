# 19 — `std::string` UDL (`""s`)

> Spec: <https://en.cppreference.com/cpp/string/basic_string/operator%22%22s>

## What you learn

C++14 added a one-character UDL that converts a string
literal directly into `std::string`:

```cpp
using namespace std::string_literals;

auto s = "hello"s;             /* s is std::string */
auto concat = "a"s + "b";      /* "ab" -- works because LHS is string */
```

Before this, `"hello"` was `const char *`, and you'd write
`std::string("hello")` (verbose) or `std::string{"hello"}` (also
verbose) to convert. The UDL collapses it to one character.

### Why this matters

The main use is **string concatenation involving literals**:

```cpp
/* C++11: doesn't compile -- pointer arithmetic, not concat */
auto bad = "a" + "b";

/* C++11 manual workaround */
auto ok11 = std::string("a") + "b";

/* C++14 UDL form */
auto ok14 = "a"s + "b";        /* std::string + const char* OK */
```

`const char*` doesn't define `operator+(const char*, const
char*)`. The expression `"a" + "b"` is pointer arithmetic
(adding an array's decay to another array's decay) -- a
compile error.

`std::string + const char*` IS defined. Once one side is a
`std::string`, the other can be a string literal. The `s`
UDL is the cleanest way to make one side a `std::string`.

### The `s` UDL is in `std::string_literals`

```cpp
namespace std::string_literals      /* inline subspace of std::literals */
namespace std::literals             /* same UDLs, broader scope */
namespace std::literals::string_literals
```

To use:

```cpp
using namespace std::string_literals;   /* just the string UDLs */
/* or */
using namespace std::literals;          /* string + chrono + complex UDLs */
```

The latter is fine for a TU that uses several UDL families.
For a single-family TU, the narrower form is clearer.

### Type details

`"hello"s` produces a `std::string`. There are similar
suffixes for:

| Suffix     | Type                  |
|------------|-----------------------|
| `""s`      | `std::string`         |
| `""sv` (C++17)| `std::string_view` |
| `u8""s`    | `std::u8string` (C++20) |
| `u""s`     | `std::u16string` (C++14) |
| `U""s`     | `std::u32string` (C++14) |
| `L""s`     | `std::wstring` (C++14)   |

For our C++14 code, the narrow `std::string` is the common
case.

### Embedded nulls

`std::string("abc\0def")` truncates at the first null:
`size() == 3`. `"abc\0def"s` also truncates, because the UDL
operator takes a `const char *` and a length, and the length
is computed via `strlen`.

To preserve embedded nulls, you need to use the explicit
`std::string` constructor with an explicit length:

```cpp
auto s = std::string("abc\0def", 7);    /* size 7 */
```

The UDL doesn't help here -- you'd use the explicit form.

### Disambiguation: `s` is a chrono suffix too

Step 18 covered the chrono `s` (seconds) UDL. The two are
disambiguated by the literal's grammar:

- `1s` -- numeric literal -> `std::chrono::seconds(1)`.
- `"x"s` -- string literal -> `std::string("x")`.

Same suffix, different prefix types, different result. Using
`using namespace std::literals;` brings in BOTH:

```cpp
using namespace std::literals;
auto t = 50ms;          /* chrono */
auto s = "hello"s;      /* string */
auto c = 1.0 + 2i;      /* complex */
```

### Why the suffix is just `s` (no underscore)

C++ reserves user-suffix names starting with `_`. The
standard library can use any suffix (including bare letters)
because it controls the namespace. User-defined UDLs MUST
start with `_` (`100_kg`, `42_px`).

The `s` is short and visible. The fact that it conflicts
with the chrono `s` is intentional -- they're meant to be
the natural way to write strings and durations,
disambiguated by the prefix type.

### vs `std::string{...}`

These three forms produce the same `std::string`:

```cpp
auto a = std::string("hello");
auto b = std::string{"hello"};
auto c = "hello"s;
```

The UDL is the shortest. For inline contexts (lambdas,
function args, initializer lists), it's much easier to read:

```cpp
auto greeting = [](const std::string &name) {
    return "Hello, "s + name + "!";
};
```

vs.

```cpp
auto greeting = [](const std::string &name) {
    return std::string("Hello, ") + name + "!";
};
```

The UDL form keeps the concatenation natural; the explicit
form makes the literal stand out as awkward.

## Task

In `solution.hpp`, fill in three function bodies:

```cpp
inline std::string hello_world()
{
    return "hello"s + " world";
}

inline std::string greeting(const std::string &name)
{
    return "Hello, "s + name + "!";
}

inline std::size_t size_of_hello()
{
    return "hello"s.size();   /* 5, not 6 */
}
```

`using namespace std::string_literals;` is already in the
scaffold.

## Run

```sh
./verify.sh
```

## Hints

- `<string>` is the header. Already included.

- The UDL operator `""s` is defined in
  `std::string_literals` (and also in `std::literals`). The
  scaffold opens the right namespace.

- `"hello"s + " world"` works because the LHS is
  `std::string`. Use the `s` suffix on the FIRST literal
  involved in concatenation, and the rest can be plain
  `const char *`.

- `"hello"s.size()` is 5 -- the size of the std::string
  (without the null terminator).

- `sizeof("hello")` (no UDL) is 6 -- the size of the char
  array (with the null terminator).

- The stub's `sizeof("hello")` returns 6. The reference's
  `"hello"s.size()` returns 5.

- The "plain_s_suffix_yields_string" test pins that `"abc"s`
  has type `std::string`. The "s_suffix_handles_embedded_null"
  test demonstrates that the UDL inherits std::string's
  embedded-null handling (the explicit (ptr, size) form
  preserves; the UDL still truncates because it computes
  size via strlen-equivalent).

- For long strings, prefer raw string literals + UDL:
  `R"(multi-line)"s` -- the parens-delimited raw form plus
  the UDL.

- C++17 added `""sv` for `std::string_view`. For C++14 you
  only have `""s` for `std::string`. Use the right one based
  on whether you need ownership.
