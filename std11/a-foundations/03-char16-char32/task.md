# 03 — `char16_t` and `char32_t`

> Spec: <https://en.cppreference.com/cpp/language/types>

## What you learn

C++11 added two new built-in character types for Unicode:

- **`char16_t`** — at least 16 bits, intended to hold one UTF-16 code unit.
- **`char32_t`** — at least 32 bits, intended to hold one UTF-32 code unit.

Both are *distinct* types from `char`, `short`, `int`, and from each
other. They get matching string-literal prefixes:

| Literal       | Element type                                | Encoding      |
|---------------|---------------------------------------------|---------------|
| `"abc"`       | `const char[N]`                             | implementation-defined narrow encoding |
| `u8"abc"`     | `const char[N]` (C++11) / `const char8_t[N]` (C++20) | UTF-8 |
| `u"abc"`      | `const char16_t[N]`                         | UTF-16        |
| `U"abc"`      | `const char32_t[N]`                         | UTF-32        |
| `L"abc"`      | `const wchar_t[N]`                          | platform-specific |

The lesson worth feeling: **a "character" a user sees is not always
one code unit**. Anything above U+FFFF (the Basic Multilingual Plane
ceiling) — most emoji, many CJK extensions — needs a *surrogate pair*
in UTF-16: two `char16_t` units for one Unicode code point. UTF-32
sidesteps this: every code point is exactly one `char32_t` unit.

## Task

Implement two functions in `solution.hpp` inside namespace `task03`:

```cpp
std::size_t count_utf16_units(const char16_t *s);
std::size_t count_utf32_units(const char32_t *s);
```

Each returns the number of non-null code units in a null-terminated
string of that type — the equivalent of `strlen`, but for the wider
character types. Crucially they count **code units**, not Unicode
**code points** — that's the whole point of the surrogate-pair case
in the test table below.

| Call                                       | Must return | Why                       |
|--------------------------------------------|-------------|---------------------------|
| `count_utf16_units(u"")`                   | `0`         | empty                     |
| `count_utf16_units(u"abc")`                | `3`         | 3 ASCII chars             |
| `count_utf16_units(u"a★b")` *(★ = U+2605)*  | `3`         | ★ is BMP, fits in 1 unit  |
| `count_utf16_units(u"🎉")` *(U+1F389)*       | `2`         | supplementary plane → surrogate pair |
| `count_utf32_units(U"")`                   | `0`         | empty                     |
| `count_utf32_units(U"abc")`                | `3`         | 3 ASCII chars             |
| `count_utf32_units(U"a★b")`                | `3`         | same as UTF-16 here       |
| `count_utf32_units(U"🎉")`                  | `1`         | UTF-32 → 1 unit per code point |

`tests.cpp` also pins type guarantees with `static_assert`:
`sizeof(char16_t) >= 2`, `sizeof(char32_t) >= 4`, and distinctness
from `char`/`wchar_t`.

## Run

```sh
./verify.sh
```

## Hints

- A plain `while (*s) { ++s; ++n; }` loop works. The null terminator
  is just `0` cast to the appropriate char type — it compares
  `false`-like in a boolean context.
- You can't use `std::strlen`: it takes `const char *`. Write the
  loop by hand.
- `<cstddef>` provides `std::size_t`.
- Both functions have the same shape — only the pointer type differs.
- You are *not* asked to decode UTF-16 here. Just count units. The
  surrogate-pair case is a property of the literal, not of your code.
