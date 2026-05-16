# 04 — Unicode string literals: `u8"..."`, `u"..."`, `U"..."`, `R"(...)"`

> Spec: <https://en.cppreference.com/cpp/language/string_literal>

## What you learn

C++11 added four new string-literal forms alongside plain `"..."` and
the pre-existing wide form `L"..."`:

| Prefix    | Element type (C++11) | Encoding                          |
|-----------|----------------------|-----------------------------------|
| `"..."`   | `const char[]`       | implementation-defined narrow     |
| `u8"..."` | `const char[]`       | **UTF-8** (deterministic bytes)   |
| `u"..."`  | `const char16_t[]`   | UTF-16                            |
| `U"..."`  | `const char32_t[]`   | UTF-32                            |
| `L"..."`  | `const wchar_t[]`    | platform-specific (pre-C++11)     |

Plus a *raw* form, **`R"delim(content)delim"`**, orthogonal to
encoding. Inside the parens, every character is taken literally — no
`\n`, `\\`, or `\"` escape processing. The delimiter can be any
character sequence (typically empty, but use `R"x(...)x"` if your
content contains the sequence `)"`). It combines with the encoding
prefixes: `u8R"(...)"`, `uR"(...)"`, `UR"(...)"`, `LR"(...)"`.

## Two things worth feeling

1. **`u8` is UTF-8 in bytes, not "ASCII + extras".** One non-ASCII
   code point uses multiple bytes:

   | Character    | Code point  | UTF-8 bytes        |
   |--------------|-------------|--------------------|
   | `é`          | U+00E9      | `0xC3 0xA9` (2)    |
   | `€`          | U+20AC      | `0xE2 0x82 0xAC` (3) |
   | `🎉`         | U+1F389     | `0xF0 0x9F 0x8E 0x89` (4) |

   So `strlen(u8"héllo")` is `6`, not `5`.

2. **Raw literals are about source-code legibility, not runtime.**
   `R"(\d+)"` and `"\\d+"` produce *byte-for-byte identical* strings
   at runtime. The raw form just saves you the backslash dance in the
   source file.

## Task

In `solution.hpp`, implement four functions inside namespace `task04`:

```cpp
const char     *as_utf8();
const char16_t *as_utf16();
const char32_t *as_utf32();
const char     *as_raw();
```

| Function     | Should return                                       |
|--------------|-----------------------------------------------------|
| `as_utf8()`  | UTF-8 encoding of `"héllo"` — 6 bytes + null        |
| `as_utf16()` | UTF-16 encoding of `"héllo"` — 5 code units + null  |
| `as_utf32()` | UTF-32 encoding of `"héllo"` — 5 code units + null  |
| `as_raw()`   | the 8 characters `\d+\.\d+` (a literal regex)       |

The return type forces the literal prefix for `as_utf16` and
`as_utf32` (no other prefix produces those pointer types). For
`as_utf8`, use the `u8` prefix so the bytes are deterministic
regardless of source-file encoding. For `as_raw`, use the raw form
so you don't have to escape the backslashes.

`tests.cpp` also pins literal sizes at compile time with `static_assert`.

## Run

```sh
./verify.sh
```

## Hints

- `é` is the e-acute Unicode escape. It works inside any literal
  prefix: `"é"`, `u8"é"`, `u"é"`, `U"é"`. Using
  it (instead of typing `é` directly) keeps your source pure ASCII,
  and the test file does exactly that.
- The string `\d+\.\d+` contains *only* backslashes, letters, dots,
  and pluses — no `)"` sequence — so you can use the empty raw
  delimiter: `R"(...)"`. The custom-delimiter form (`R"foo(...)foo"`)
  exists for content that itself contains `)"`.
- `std::char_traits<char16_t>::length` and the `char32_t` version
  give you the equivalent of `strlen` for the wider types (you wrote
  this loop by hand in step 03).
