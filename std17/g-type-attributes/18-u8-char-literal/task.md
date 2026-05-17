# 18 — `u8` character literal

> Spec: <https://en.cppreference.com/cpp/language/character_literal>

## What you learn

C++17 added `u8'X'` for **UTF-8 character literals**. The
value is the UTF-8 code unit for `X`. In C++17, the type is
`char`. (C++20 changed it to `char8_t`.)

For ASCII characters, `u8'A' == 'A' == 65`.

```cpp
constexpr char a = u8'A';   /* 65 */
constexpr char z = u8'0';   /* 48 */
```

For multi-byte UTF-8 characters, `u8'X'` is ill-formed if X
doesn't fit in a single code unit (i.e., not ASCII).

## Task

Return the values for `'A'`, `'0'`, `'\n'` via `u8'...'`.

## Run

```sh
./verify.sh
```

## Hints

- `u8'A'`, `u8'0'`, `u8'\n'` -- all single-code-unit ASCII.

- In C++17, the type is `char`. In C++20, `char8_t`.

- `u8'é'` (e-acute) is **ill-formed** in C++17 because
  it requires 2 UTF-8 bytes. Use a `u8""` string literal
  for multi-byte UTF-8.
