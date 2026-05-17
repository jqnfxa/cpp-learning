# 125 — `std::regex` basics

> Spec: <https://en.cppreference.com/cpp/regex>

## What you learn

C++11's `<regex>` is the standard library's regular-expression
engine. The core type is `std::regex` (a compiled pattern), and
the three core algorithms are:

| Function                        | Effect                                |
|---------------------------------|---------------------------------------|
| `std::regex_match(str, re)`     | Does the WHOLE string match `re`?     |
| `std::regex_search(str, re)`    | Does any part match?                  |
| `std::regex_replace(str, re, s)` | Replace matches with `s`.            |

For "validate that the entire input looks like X" the right
choice is `regex_match`.

```cpp
std::regex re(R"([A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,})");
bool ok = std::regex_match(s, re);
```

### Why `R"(...)"` raw strings

C++ regex patterns are full of backslashes. Without raw
strings, you'd write every `\d` as `"\\d"`, every `\s` as
`"\\s"`. The C++11 raw string literal `R"(...)"` lets you
write the pattern verbatim:

```cpp
"\\d+\\.\\s*\\w+"        /* without raw strings */
R"(\d+\.\s*\w+)"          /* with raw strings */
```

This step's task uses an email-shaped pattern that has a literal
`\.` (escaped period). Raw strings make it readable.

### The default grammar: ECMAScript

`std::regex` defaults to the **ECMAScript** flavour (a subset
of JavaScript's regex syntax). Other grammars are available
via the constructor's second argument:

```cpp
std::regex re("...", std::regex::extended);    /* POSIX extended */
std::regex re("...", std::regex::basic);       /* POSIX basic */
std::regex re("...", std::regex::awk);          /* awk */
std::regex re("...", std::regex::grep);         /* grep */
std::regex re("...", std::regex::egrep);        /* egrep */
```

For most C++ code, ECMAScript is what you want. Its features:

- `\d`, `\w`, `\s` character classes.
- `?`, `+`, `*` quantifiers.
- `{n}`, `{n,m}` bounded quantifiers.
- `()` capture groups (step 126).
- `[]` character sets.
- `|` alternation.

### Construction is expensive; reuse it

Compiling a regex (the constructor of `std::regex`) is slow --
it parses the pattern, builds an NFA. **Don't put the
construction inside a hot loop.**

The idiomatic spelling for "validate a stream of inputs against
a fixed pattern":

```cpp
bool is_email(const std::string &s)
{
    static const std::regex re(R"(...)");    /* compiled ONCE */
    return std::regex_match(s, re);
}
```

The `static const` makes it a function-local-static -- compiled
on first call, reused forever. C++11 guarantees thread-safe
initialisation of function-local statics ("magic statics"), so
this is safe even from multiple threads.

### A "good enough" email regex

Production-grade email validation is *much* harder than the
common regexes you'll see. The RFC 5322 grammar is famously
unworkable in one regex. For most uses, "matches
`local@domain.tld`" is enough:

```
[A-Za-z0-9._%+-]+      # local part: letters/digits/some symbols
@                      # literal at
[A-Za-z0-9.-]+         # domain part
\.[A-Za-z]{2,}         # final ".xyz" with at least 2 letters
```

This rejects:
- `"alice"` (no @).
- `"@example.com"` (empty local part).
- `"missing"` (no @).
- `"missing@tld"` (no `.xx`).

It accepts:
- `"alice@example.com"`
- `"bob.smith@foo.bar.org"`
- `"user+tag@domain.io"`

It will accept things real email systems wouldn't, but for a
"is this email-shaped?" sniff, it's adequate.

## Task

In `solution.hpp`, implement one function:

```cpp
namespace task125
{
    /* True if s matches the simple email pattern
     * "<local>@<domain>.<tld>" where local and domain are
     * alphanumeric-ish and tld is 2+ letters. */
    bool is_email_shaped(const std::string &s);
}
```

## Run

```sh
./verify.sh
```

## Hints

- Use a `static const std::regex` inside the function to avoid
  recompiling on every call.

- The pattern (one popular variant):
  ```cpp
  R"([A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,})"
  ```

- `std::regex_match(s, re)` requires the *whole* string to
  match. That's what email validation should do; you don't
  want `"is alice@example.com a member?"` to match.

- The character class `[A-Za-z0-9._%+-]` is a set: the
  hyphen at the end is a literal `-` (placing it last avoids
  ambiguity with ranges). The dot and other punctuation
  inside `[...]` are literal.

- `\.` inside the raw string is the literal period (because
  `.` alone matches any character).

- `{2,}` means "two or more". Some regex flavours need this
  spelled differently; ECMAScript and most others accept it
  as-is.

- The tests rely on exact-match semantics. A string like
  `"alice@example.com extra"` must return false (extra text
  after the email).

- For a real production email check, use a parser library or
  send a verification email -- regex validation gives false
  negatives (rejects valid esoteric addresses) and false
  positives (accepts shapes that don't actually resolve). The
  scope here is "is the string roughly the shape of an
  email?".
