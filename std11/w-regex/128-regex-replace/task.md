# 128 — `std::regex_replace`

> Spec: <https://en.cppreference.com/cpp/regex/regex_replace>

## What you learn

`std::regex_replace` does **substitution**: find every match
of a regex in the source string and replace it with a
*format string*. By default, it returns a fresh string with
the substitutions applied -- the input is unchanged.

```cpp
std::string s = "abc 12 def 34";
std::regex digits(R"(\d+)");
std::string out = std::regex_replace(s, digits, "[N]");
/* out == "abc [N] def [N]" */
```

### The shape

```cpp
std::string regex_replace(const std::string &source,
                          const std::regex &re,
                          const std::string &fmt);
```

Walks `source`, for each match of `re` substitutes `fmt`, and
returns the resulting string. Non-matching parts are copied
verbatim. There's a `flags` parameter too (default = "replace
all occurrences").

### Format-string substitution

The third argument is a **format string** that can reference
the match and captures with `$` syntax:

| Token  | Meaning                          |
|--------|----------------------------------|
| `$0`   | The whole match (same as `$&`).  |
| `$1`   | Capture group 1.                 |
| `$2`   | Capture group 2.                 |
| ...    | ...                              |
| `$&`   | The whole match.                 |
| ``$` ``| The text *before* the match.     |
| `$'`   | The text *after* the match.      |
| `$$`   | A literal `$`.                   |

So you can rewrite "host:port" pairs:

```cpp
std::regex re(R"(([^:]+):(\d+))");
std::string out = std::regex_replace(s, re, "$2 at $1");
/* "localhost:8080" -> "8080 at localhost" */
```

### The "replace all" default

`regex_replace` replaces **every** non-overlapping match by
default. To replace only the first, pass
`std::regex_constants::format_first_only`:

```cpp
std::regex_replace(s, re, "X",
                   std::regex_constants::format_first_only);
```

### Redaction pattern

The textbook redaction case: replace every digit with a
sentinel character. `\d` matches `[0-9]`; the format string is
just the literal `#` with no `$` references:

```cpp
std::string redact_digits(const std::string &s)
{
    static const std::regex digits(R"(\d)");
    return std::regex_replace(s, digits, "#");
}
```

Output: `"call 555-1234"` -> `"call ###-####"`. Each digit
becomes one `#`; non-digits pass through unchanged.

### Why not `\d+`?

`\d` (one digit per replacement) keeps the digit count visible
-- "555" becomes "###", preserving length. With `\d+` (one
replacement per digit run), "555" would become "#", collapsing
length. For redaction-while-preserving-shape, `\d` is the
choice.

### Static const, again

Same reason as in steps 125-127: compiling a regex is
expensive, the pattern doesn't change, so hoist it to a
`static const` inside the function:

```cpp
static const std::regex digits(R"(\d)");
```

This is the canonical regex-in-hot-path idiom in C++11.

### Other useful forms

```cpp
/* Lowercase->dash: convert spaces to '-' */
std::regex_replace(s, std::regex(R"(\s+)"), "-");

/* Strip HTML tags */
std::regex_replace(s, std::regex("<[^>]+>"), "");

/* Insert thousand-separators (needs lookbehind in real life,
 * but the basic idea is: match digit-groups, replace with
 * "$0,"). */
```

There's also an output-iterator form
(`regex_replace(out_it, begin, end, re, fmt)`) for streaming
into a buffer or `std::ostream_iterator`. The string-returning
form is the common case.

## Task

In `solution.hpp`, implement one function:

```cpp
namespace task128
{
    /* Return a copy of s with every digit replaced by '#'. */
    std::string redact_digits(const std::string &s);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The body:
  ```cpp
  static const std::regex digits(R"(\d)");
  return std::regex_replace(s, digits, "#");
  ```

- Use `\d` (single digit) rather than `\d+` (digit-run) so a
  three-digit number redacts to three `#`s, preserving length.

- The format string is just `"#"` -- no `$` substitutions
  needed. If you accidentally write `"$0"`, you'd reinsert
  the matched digit. If you write `"$$"`, you'd get a literal
  `$` per digit.

- `regex_replace` returns a NEW string. The input is `const`
  and unchanged. (There's no in-place form in C++11 -- assign
  the result back if you need it.)

- Empty input -> empty output. The regex finds no matches and
  the result is an empty string.

- Non-ASCII bytes (`utf-8` continuation bytes etc.) aren't `\d`
  in ECMAScript -- they pass through unchanged. ECMAScript
  `\d` is `[0-9]` only; it does NOT match Devanagari digits,
  fullwidth digits, etc.

- Tests cover: single digit, all digits, no digits
  (passthrough), mixed content, empty input, phone-number
  shape, leading/trailing digits, embedded special characters
  ($, &, \) which are NOT activated because they're in the
  INPUT, not the format string.
