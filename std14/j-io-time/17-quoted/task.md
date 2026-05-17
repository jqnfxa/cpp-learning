# 17 — `std::quoted`

> Spec: <https://en.cppreference.com/cpp/io/manip/quoted>

## What you learn

`std::quoted` is a C++14 **stream manipulator** for
quoting/unquoting strings during stream I/O:

- On output: wraps the string in double quotes, escapes any
  embedded `"` or `\` with `\`.
- On input: reads a double-quoted token, undoing the escapes.

```cpp
std::stringstream ss;
ss << std::quoted("hello world");
std::cout << ss.str();      /* "hello world" */

std::string back;
ss >> std::quoted(back);
std::cout << back;          /* hello world */
```

The core problem it solves: default `operator>>` splits on
whitespace. Reading a string with `>> s` only captures up to
the first space. `std::quoted` reads until the closing
double-quote, preserving everything between.

### When to use it

- **Round-trip serialization** of string fields containing
  spaces or quotes (config files, log lines, simple CSV
  alternatives).
- **Generating quoted output** for JSON-style payloads
  (limited; std::quoted doesn't escape control chars).
- **Parsing tokens** that may contain spaces.

### Customisation

```cpp
std::quoted(s)             /* default: delimiter `"`, escape `\` */
std::quoted(s, '|')        /* use `|` instead of `"` as delimiter */
std::quoted(s, '"', '%')   /* use `%` as escape instead of `\` */
```

Useful when the data could contain `"` as a normal character
(e.g., human language with curly quotes; you might pick a
non-printable as delimiter).

### Round-trip mechanics

The key property: `serialize` then `deserialize` returns the
original string exactly, even if it contains:

| Input         | Serialised      | Deserialised |
|---------------|-----------------|--------------|
| `hello`       | `"hello"`       | `hello`      |
| `hi world`    | `"hi world"`    | `hi world`   |
| `say "hi"`    | `"say \"hi\""`  | `say "hi"`   |
| `a\b`         | `"a\\b"`        | `a\b`        |
| `` (empty)    | `""`            | `` (empty)   |

The escape rules:
- `"` -> `\"` (so the delimiter doesn't end the string)
- `\` -> `\\` (so an interior backslash doesn't escape the
  next char unintentionally)

### What it doesn't do

- **Control characters**: tabs and newlines pass through
  verbatim. If you write `"a\tb"`, the quoted form contains
  a literal tab. JSON requires `\t`, `\n`, etc. -- std::quoted
  doesn't do that.
- **Unicode escaping**: non-ASCII bytes pass through verbatim
  (UTF-8 is preserved byte-by-byte, but no `\uXXXX` form).
- **JSON output**: not designed for it. Use a real JSON
  library if you need JSON.

For "simple round-trip of strings with embedded spaces and
quotes," std::quoted is the right tool.

### Use as both manipulator and "manipulator-with-arg"

`std::quoted` is unusual: it's a function that takes a
string and returns an object whose `operator<<` writes the
quoted form. On the input side, `std::quoted(s)` returns an
object whose `operator>>` reads back into `s`.

```cpp
ss << std::quoted(s);    /* write: serialise */
ss >> std::quoted(out);  /* read: parse and unescape */
```

No state is held in the manipulator -- each invocation is
fresh.

### Use cases for the second argument (delimiter)

Two common patterns:

```cpp
/* Default double-quote */
ss << std::quoted(filename);     /* "my file.txt" */

/* Use single quote when " is part of the value */
ss << std::quoted(quote_text, '\'');   /* 'he said "hi"' */
```

For data that mixes both quote types, std::quoted isn't a
great fit -- a real escape format (JSON, CSV, custom) handles
the mixed case better.

### Mixed with other manipulators

`std::quoted` is just one of many stream manipulators (like
`std::setw`, `std::hex`). They compose:

```cpp
ss << "filename=" << std::quoted(name)
   << " size=" << size
   << " hex=" << std::hex << value;
```

This is how you'd write a key=value log line where one
value contains spaces.

### vs `std::getline` / manual parsing

The pre-C++14 patterns:

- `std::getline(ss, s)` reads until newline -- works for
  one field per line, but doesn't handle multiple
  space-containing fields on one line.
- Manual parsing: count quotes, handle escapes by hand.
  Error-prone.

`std::quoted` is a small but useful step up: one line for the
common "I have a string field with spaces" case.

## Task

In `solution.hpp`, fill in two functions:

1. `serialize(s)`: write `s` through `std::quoted` to an
   ostringstream and return the result.

2. `round_trip(s)`: write through `std::quoted` to a
   stringstream, then read back through `std::quoted`,
   returning the deserialised value (should equal `s`).

```cpp
inline std::string serialize(const std::string &s) {
    std::ostringstream oss;
    oss << std::quoted(s);
    return oss.str();
}

inline std::string round_trip(const std::string &s) {
    std::stringstream ss;
    ss << std::quoted(s);
    std::string out;
    ss >> std::quoted(out);
    return out;
}
```

## Run

```sh
./verify.sh
```

## Hints

- `<iomanip>` is the header for `std::quoted`. Already
  included.

- `std::ostringstream` is the output-only string stream;
  use `.str()` to get the result.

- `std::stringstream` is bidirectional -- for round-trip
  testing we need both write and read on the same stream.

- The serialised form is the QUOTED form -- double quotes
  around the value plus `\"` escapes for embedded quotes
  and `\\` for embedded backslashes. The "serialize"
  function returns those quotes as part of the result.

- `round_trip` should be a no-op semantically: write quoted,
  read quoted, get back what you wrote.

- The naive `ss << s` (no manipulator) writes just the
  string contents. The naive `ss >> s` reads until whitespace
  -- so "hello world" round-trips as just "hello." This is
  the stub's bug.

- For embedded quotes (`say "hi"`), the serialised form is
  `"say \"hi\""`. The interior `"`s are escaped with `\`.
  Round-tripping recovers the original.

- For embedded backslashes (`a\b`), the serialised form is
  `"a\\b"`. The interior `\` is doubled.

- Tabs and newlines pass through verbatim (no `\t`/`\n`
  escapes). std::quoted is NOT JSON.

- Unicode bytes (UTF-8) pass through verbatim. The
  serialized form preserves them byte-by-byte.

- Tests verify: serialize values for simple/spaces/embedded-
  quote/embedded-backslash/empty; round_trip for the same
  + tabs+punctuation + UTF-8 bytes.

- C++14 introduced `std::quoted`; no equivalent change in
  later standards (C++17's `<charconv>` is different).
