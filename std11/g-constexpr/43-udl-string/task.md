# 43 — User-defined literals: string

> Spec: <https://en.cppreference.com/cpp/language/user_literal>

## What you learn

String literal UDLs have yet another fixed cooked-form parameter
shape: **`(const char *str, std::size_t len)`**. The compiler hands
you the pointer to the literal and its length (not including the
null terminator). You return whatever type you like.

```cpp
std::vector<unsigned char> operator"" _bytes(const char *str, std::size_t len)
{
    return std::vector<unsigned char>(str, str + len);
}

auto blob = "AB\x01"_bytes;   /* vector of 3 bytes: 0x41, 0x42, 0x01 */
```

Notes:

1. **The function is *not* `constexpr`.** Most return types
   (`std::vector`, `std::string`) allocate memory and can't be
   built in C++11's `constexpr` rules. C++20 finally allows
   `constexpr std::vector` but that's later.

2. **`u8`, `u`, `U`, `L`, and `R` prefixes route to different
   UDLs** if you define them. For now we only handle the plain
   narrow string literal `"..."`.

3. **The standard library uses string UDLs**: `"hello"s` (with `s`
   suffix) builds a `std::string`. The `s` suffix is reserved for
   the standard (no underscore prefix needed for std-library
   suffixes); your suffixes must start with `_`.

## Task

In `solution.hpp`, implement two string UDLs:

```cpp
/* Treat the literal's bytes as raw data. Length is the number of
 * bytes excluding the null terminator. */
std::vector<unsigned char> operator"" _bytes(const char *str, std::size_t len);

/* Return the length (number of chars, excluding null terminator).
 * Useful trivia: lets you write `auto n = "hello"_len;` instead of
 * `strlen("hello")`. */
std::size_t operator"" _len(const char *str, std::size_t len);
```

| Literal              | Returns                              |
|----------------------|--------------------------------------|
| `""_bytes`           | empty vector                         |
| `"A"_bytes`          | `{0x41}`                              |
| `"AB"_bytes`         | `{0x41, 0x42}`                        |
| `"\x01\xff"_bytes`   | `{0x01, 0xff}`                        |
| `""_len`             | `0`                                   |
| `"hello"_len`        | `5`                                   |
| `"a\0b"_len`         | `3` (embedded null is part of length) |

## Run

```sh
./verify.sh
```

## Hints

- `_bytes` body:
  ```cpp
  return std::vector<unsigned char>(str, str + len);
  ```
  `std::vector` has an iterator-pair constructor. `str` is `const
  char *`, but `std::vector<unsigned char>` accepts the range and
  converts each element from `char` to `unsigned char`.

- `_len` body:
  ```cpp
  return len;
  ```
  Yes, that's it. The whole point is that the *compiler* gives you
  the length for free — no `strlen` runtime cost.

- For the embedded-null test: a string literal `"a\0b"` is **3
  characters long** (plus the implicit trailing `\0` for null
  termination). String UDLs receive that length as `len`, ignoring
  embedded nulls.

- Like the previous UDL steps, the result of the UDL must be
  brought into scope (`using namespace task43;` in `tests.cpp` or
  similar).
