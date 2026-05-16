# 13 — `enum class` with explicit underlying type

> Spec: <https://en.cppreference.com/cpp/language/enum>

## What you learn

In step 12 you saw that `enum class` has a default underlying type of
`int`. C++11 also lets you **pin** the underlying type with a
trailing `: <integer_type>`:

```cpp
enum class status : std::uint8_t { ok, fail };           /* 1 byte */
enum class field  : std::uint16_t { one, two, three };   /* 2 bytes */
enum class id     : std::uint64_t { sentinel = ~0ULL };  /* 8 bytes */
```

Why pin the type?

1. **Size predictability.** For wire formats, persistence layouts, or
   constrained-memory data, you need to know how many bytes the enum
   occupies. Default `int` is fine for in-memory code, but a 4-byte
   status flag wastes space in a packed struct.

2. **Forward declarability.** A scoped enum can be forward-declared
   *only* if its underlying type is known — which means either
   defaulted (`enum class color;` always defaults to `int`) or pinned
   (`enum class status : std::uint8_t;`). The pinned form is what you
   typically reach for in headers that just need a pointer or reference
   to the type without seeing the enumerator list.

3. **Range control.** `std::uint8_t` lets you hold values up to 255;
   `std::int8_t` only up to 127. You make the choice deliberately.

## Task

In `solution.hpp`, declare three scoped enums inside `task13`:

| Name      | Underlying type   | Enumerators                                |
|-----------|-------------------|--------------------------------------------|
| `status`  | `std::uint8_t`    | `ok = 0`, `fail = 1`                       |
| `field`   | `std::uint16_t`   | `one`, `two`, `three`                      |
| `id`      | `std::uint64_t`   | `sentinel = static_cast<std::uint64_t>(-1)`|

`tests.cpp` pins:
- `sizeof(status) == 1`
- `sizeof(field) == 2`
- `sizeof(id) == 8`
- `underlying_type<E>::type` matches the pinned type for each.

Plus runtime checks that the enumerator values are correct (in
particular that `id::sentinel` round-trips through `std::uint64_t` as
`~0ULL`).

## Run

```sh
./verify.sh
```

## Hints

- The syntax is `enum class NAME : UNDERLYING_TYPE { enumerators... };`.
- `<cstdint>` provides `std::uint8_t`, `std::uint16_t`, `std::uint64_t`.
  Include it in `solution.hpp`.
- For `id::sentinel`, the value `-1` doesn't fit a *signed* underlying
  type narrower than 64 bits, but it casts cleanly into `std::uint64_t`
  as `0xFFFF'FFFF'FFFF'FFFF`. Use either:
  ```cpp
  sentinel = static_cast<std::uint64_t>(-1)
  /* or */
  sentinel = ~0ULL
  ```
- For `status::ok = 0`, the explicit `= 0` is optional (the first
  enumerator defaults to 0 anyway), but it's nice to spell it out
  when the value carries meaning.
