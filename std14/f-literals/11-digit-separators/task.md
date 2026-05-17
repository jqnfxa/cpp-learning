# 11 — Digit separators (`'`)

> Spec: <https://en.cppreference.com/cpp/language/integer_literal>

## What you learn

C++14 added the **single quote** as an optional digit
separator inside numeric literals. The compiler ignores it
when parsing the value:

```cpp
1'000'000          /* 1000000 -- one million */
0xAA'BB'CC'DD      /* 0xAABBCCDD */
0b1100'0000'1010'1000  /* 0xC0A8 */
3'141'592          /* 3141592 */
```

Separators have **no semantic effect** -- the value is
exactly what you'd write without them. They exist purely to
make long literals readable.

### Where you can put them

Between any two **digits** of a numeric literal. The rules:

- After the base prefix (`0b`, `0x`, `0`): yes, just before
  the first digit. Example: `0x'AB` is **not** valid (the
  separator can't precede the first digit).
- Between adjacent digits in the main number: yes.
- Before the suffix (`u`, `l`, `ll`, `f`): no -- the
  separator must be between digits, not between a digit and
  a suffix letter.

Valid:
```cpp
1'000'000      1'2'3'4'5      0xDEAD'BEEF      0b1010'0011
```

Invalid:
```cpp
'1000           /* before first digit */
1000'           /* after last digit */
1000'u          /* between digit and suffix */
0x'DEADBEEF     /* between prefix and first digit */
```

### Floating-point too

Separators work in floating-point literals as well, on both
sides of the decimal point and around the exponent:

```cpp
3.141'592'653       /* pi */
1.0e1'000           /* exponent grouping */
1'000.000'1         /* both sides of decimal */
```

The same "between digits" rule applies. Exponent `e` /
floating-point suffix `f` / `l` are not digits, so
separators can't appear directly adjacent to them.

### What separator grouping looks like in practice

Conventionally:

- **Decimal**: groups of 3 from the right.
  `1'000'000`, `1'234'567'890`.
- **Hex**: groups of 4 (one 16-bit word) or 2 (one byte).
  `0xDEAD'BEEF`, `0xAA'BB'CC'DD`.
- **Binary**: groups of 4 (one nibble) or 8 (one byte).
  `0b1010'0011`, `0b1010'0011'0101'0101`.

These are conventions, not rules. The compiler doesn't care
where you put the separators as long as they're between
digits.

### Type still follows the literal

```cpp
auto a = 1'000'000;          /* int */
auto b = 1'000'000LL;        /* long long */
auto c = 1'000'000u;         /* unsigned */
auto d = 1'000'000.5;        /* double */
auto e = 1'000'000.5f;       /* float */
```

The separators don't change the type-deduction rules. The
suffix (or absence) does.

### When NOT to use separators

- **Short literals**: `42'00` is harder to read than `4200`.
  Use separators when grouping actually helps.
- **Conflicting groupings**: don't put a 3-group separator
  in a hex literal -- byte alignment is the natural choice
  there, not decimal alignment.
- **Magic numbers**: a `constexpr` named constant is almost
  always better than a literal with separators, regardless
  of how readable the literal is.

### Comparison to other languages

The separator was borrowed from Ada (which uses `_`) and
similar features in Java 7+ (`_`), Python 3.6+ (`_`),
JavaScript ES2021 (`_`). C++ went with `'` because `_` is
already a valid identifier character (and was reserved for
user-defined literals).

Trade-off: `_` is more visually neutral; `'` looks a bit
like a thousands separator (the European style) but can be
confused with a character literal. Editors highlight
differently per language.

## Task

In `solution.hpp`, set five constants using digit
separators correctly:

| Name           | Value (decimal) | With separators        |
|----------------|-----------------|------------------------|
| `million`      | 1'000'000       | `1'000'000`            |
| `billion`      | 1'000'000'000   | `1'000'000'000LL`      |
| `mac_prefix`   | 0xAABBCCDD      | `0xAA'BB'CC'DD`        |
| `subnet`       | 49320 = 0xC0A8  | `0b1100'0000'1010'1000`|
| `pi_micro`     | 3'141'592       | `3'141'592`            |

```cpp
namespace task11
{
    constexpr auto million        = 1'000'000;
    constexpr auto billion        = 1'000'000'000LL;
    constexpr std::uint32_t mac_prefix = 0xAA'BB'CC'DD;
    constexpr std::uint16_t subnet     = 0b1100'0000'1010'1000;
    constexpr auto pi_micro       = 3'141'592;
}
```

## Run

```sh
./verify.sh
```

## Hints

- The scaffold's stub values are each missing one group of
  zeros or have one bit/digit flipped.

- `1'00'000` is 100'000 (one hundred thousand), not
  1'000'000 (one million). Add another group of zeros.

- `1'00'000'000` is 100'000'000 (one hundred million). Add
  another zero to reach one billion.

- For `mac_prefix`, the stub flips the last hex digit
  (`0xAABBCCDC` instead of `0xAABBCCDD`).

- For `subnet`, the stub flips one bit
  (`0b1100'0000'1010'1001` is 49321, not 49320). The
  correct value is `0b1100'0000'1010'1000`.

- For `pi_micro`, the stub is off by one (`3'141'591` vs
  `3'141'592`). Pi to 6 decimal places is 3.141592, times
  a million is 3'141'592.

- The `LL` suffix on `billion` ensures `decltype(billion) ==
  long long` even on a 64-bit `int` system. The test
  pins this.

- Hex/binary/decimal can all use separators -- each base has
  its own conventional grouping. Hex by nibble or byte;
  binary by nibble or byte; decimal by 3 from the right.

- Tests verify: each value equals its decimal equivalent,
  the long-long type pin, mac_prefix's first byte =
  0xAA via shift+mask, subnet's two bytes via shift+mask,
  and a constexpr-evaluable check.

- Separators are stripped by the compiler before parsing the
  numeric value. There's no runtime cost. You're free to
  scatter them anywhere they help readability.
