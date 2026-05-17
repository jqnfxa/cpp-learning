# 10 — Binary literals (`0b...`)

> Spec: <https://en.cppreference.com/cpp/language/integer_literal>

## What you learn

C++14 added the `0b` (or `0B`) prefix for **binary integer
literals**:

```cpp
0b1010'0011    /* 163 -- with optional digit separators */
0b11111111     /* 255 */
0B101          /* 5  -- uppercase B also accepted */
```

Each `0b...` literal is an integer with value computed by
the obvious bit-by-bit reading. The type follows the same
rules as any other integer literal: smallest `int`-and-up
type that can hold the value, unless a suffix forces
otherwise (`u`, `l`, `ll`, etc.).

### Why this matters

Hexadecimal is OK for "this bit pattern" code (`0xFF`,
`0xA3`), but it hides the bit boundaries. For configuration
flags, masks, and protocol fields, binary literals show the
structure directly:

```cpp
/* Hex: opaque -- which bits are set? */
constexpr int flags = 0xC1;

/* Binary: bit positions visible at a glance */
constexpr int flags = 0b1100'0001;   /* bits 7, 6, 0 */
```

For a hardware register layout, an instruction encoding, a
network protocol field -- the binary form is much more
readable.

### Digit separators (paired with step 11)

The `'` character can be placed between any two digits of an
integer literal to group them:

```cpp
0b1010'0011
0b1010'0011'0101'0101    /* 16 bits grouped in nibbles */
1'000'000                /* one million, decimal */
0xDEAD'BEEF              /* 32-bit hex with byte groups */
```

Separators have no semantic effect -- the compiler strips
them before parsing the numeric value. They're purely for
readability. Step 11 covers separators in detail.

### Equivalent representations

Every integer literal has the same numeric value regardless
of base:

| Decimal | Binary           | Hexadecimal | Octal  |
|---------|------------------|-------------|--------|
| 163     | `0b1010'0011`    | `0xA3`      | `0243` |
| 255     | `0b1111'1111`    | `0xFF`      | `0377` |
| 85      | `0b0101'0101`    | `0x55`      | `0125` |
| 170     | `0b1010'1010`    | `0xAA`      | `0252` |

You can mix bases freely in expressions:
`(0b1010'0011 & 0xF0) == 0xA0`. The result type follows
standard integer-promotion rules.

### Type deduction

```cpp
auto x = 0b1010'0011;     /* int (the literal's type) */
auto y = 0b1010'0011u;    /* unsigned int  */
auto z = 0b1010'0011ll;   /* long long     */
```

With `auto`, the variable's type matches the literal's type.
For embedded / bit-manipulation code where you want a
specific width, use `<cstdint>` typedefs:

```cpp
constexpr std::uint8_t mask = 0b1010'0011;     /* explicit u8 */
```

The narrowing from `int` (literal type) to `uint8_t` is
permitted via `constexpr` if the literal value fits in 8
bits.

### What's NOT supported

- **Binary floating-point literals**: there's no `0b1.01p2`
  syntax for "binary scientific notation." Use the C99
  hexadecimal form (`0x1.4p3`) for arbitrary-precision
  floats.

- **Arbitrary-base literals**: only 2, 8, 10, 16 are
  supported. Base-3, base-5, base-7 literals don't exist.

- **Negative numeric literals**: `-0b1010` is the unary minus
  applied to `0b1010`, not a "negative binary literal." The
  parser sees `-` then a positive literal.

## Task

In `solution.hpp`, set three `constexpr auto` constants:

| Name              | Value          |
|-------------------|----------------|
| `mask`            | `0b1010'0011` (163, 0xA3) |
| `all_ones_byte`   | `0b1111'1111` (255, 0xFF) |
| `alternating`     | `0b0101'0101` (85, 0x55)  |

```cpp
namespace task10
{
    constexpr auto mask = 0b1010'0011;
    constexpr auto all_ones_byte = 0b1111'1111;
    constexpr auto alternating = 0b0101'0101;
}
```

## Run

```sh
./verify.sh
```

## Hints

- The scaffold has each constant off by one bit -- change
  the wrong bit to its correct value.

- `0b1010'0010` is `162` (last bit clear). The correct
  `mask` value `0b1010'0011` is `163` (last bit set).

- `0b1111'1110` is `254`. Correct `all_ones_byte` is
  `0b1111'1111` (255).

- `0b0101'0100` is `84`. Correct `alternating` is
  `0b0101'0101` (85).

- The digit separators (the `'` between nibbles) are
  optional but improve readability. Tests don't care whether
  you write `0b10100011` or `0b1010'0011` -- they're the
  same literal.

- `constexpr auto` deduces `int` for these values. If you
  want `unsigned int`, append `u`: `0b1010'0011u`.

- The cross-base equivalence tests verify your binary
  literal equals the hexadecimal form. `0b1010'0011 == 0xA3`
  is a useful sanity check during writing.

- The bitwise-and / bitwise-xor tests are integration
  checks: with the correct constants, `mask & 0xFF == mask`,
  and `alternating ^ all_ones == 0xAA = 170`.

- C++14 added binary literals AND digit separators in the
  same release. Step 11 covers separators in their own
  right; step 10 uses them in passing for readability.
