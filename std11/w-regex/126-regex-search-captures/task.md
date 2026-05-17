# 126 — `std::regex_search` and capture groups

> Spec: <https://en.cppreference.com/cpp/regex/regex_search>

## What you learn

Where `regex_match` requires the WHOLE input to match,
`regex_search` looks for the pattern *anywhere* inside the
string. And both forms accept a third argument -- a
`std::smatch` (a "string match result") -- that records the
captured substrings.

```cpp
std::regex re(R"((\w+):(\d+))");
std::smatch m;

std::regex_search("hello localhost:8080 world", m, re);
/* m[0] = "localhost:8080"
   m[1] = "localhost"
   m[2] = "8080"
*/
```

### `std::smatch` — the match result

A `std::smatch` (typedef for `std::match_results<std::string::const_iterator>`)
contains:

- `m[0]`: the whole matched substring.
- `m[1]`, `m[2]`, ...: capture groups, in order.
- `m.prefix()`: the part of the input *before* the match.
- `m.suffix()`: the part *after*.
- `m.size()`: number of capture groups + 1 (for `m[0]`).

Each `m[i]` is a `std::sub_match` -- a pair of iterators into
the input. You convert it to `std::string` with `.str()` or
implicit conversion when you assign it.

### Capture groups: `(...)`

In ECMAScript regex (the C++ default), parentheses make a
**capture group**. Each set of parens captures the matched
substring into the next available slot:

```cpp
R"((\w+):(\d+))"
   ^^^^^  ^^^^
   m[1]   m[2]
```

To group without capturing, use `(?:...)`:

```cpp
R"(foo(?:bar)?baz)"
   /* captures nothing; the (?:bar) is a grouping for the ?,
      not a capture */
```

For "host:port" parsing, you want two captures:
- Group 1: the host (everything before the colon).
- Group 2: the port (digits after the colon).

### `regex_match` vs `regex_search` for parsing

- **`regex_match(s, m, re)`**: the entire `s` must match the
  pattern. Captures are filled.
- **`regex_search(s, m, re)`**: the first occurrence of the
  pattern within `s` matches. Captures are filled with the
  first match's content.

For "validate AND parse", `regex_match` is cleaner -- you get
guaranteed-whole-string parsing plus the captures. For "find
the first occurrence in a larger string", use `regex_search`.

This task uses `regex_match` style: the entire input is the
"host:port" string; we want to validate the shape and extract
both halves at once.

### Converting captures to numbers

`m[2].str()` gives you a `std::string`. To get an int:

```cpp
int port = std::stoi(m[2].str());
```

`std::stoi` throws `std::invalid_argument` if the string isn't
parsable. The regex's `\d+` constraint guarantees it's digits,
but if the string was very long it might throw
`std::out_of_range`. For host:port use cases (5-digit port
max), this isn't a concern.

### Example: host:port

```cpp
bool parse_host_port(const std::string &s, std::string &host, int &port)
{
    static const std::regex re(R"(([^:]+):(\d+))");
    std::smatch m;
    if (!std::regex_match(s, m, re)) return false;
    host = m[1].str();
    port = std::stoi(m[2].str());
    return true;
}
```

- `([^:]+)`: one or more chars that are NOT a colon -- the host.
- `:`: the literal separator.
- `(\d+)`: one or more digits -- the port.

Both `host` and `port` are out-params; the return value
indicates success.

## Task

In `solution.hpp`, implement:

```cpp
namespace task126
{
    /* Parse `s` as "host:port". On success, fill `host` and `port`
     * and return true. On failure, return false (out-params unchanged
     * may have any value). */
    bool parse_host_port(const std::string &s,
                         std::string &host, int &port);
}
```

## Run

```sh
./verify.sh
```

## Hints

- Body shape:
  ```cpp
  static const std::regex re(R"(([^:]+):(\d+))");
  std::smatch m;
  if (!std::regex_match(s, m, re)) return false;
  host = m[1].str();
  port = std::stoi(m[2].str());
  return true;
  ```

- `static const std::regex re(...)` for compile-once
  amortisation (step 125 lesson).

- `std::smatch` (alias for `std::match_results<...>`) is in
  `<regex>`. Same header as `std::regex`.

- `m[1]` and `m[2]` are sub_match objects. `.str()` returns a
  `std::string` copy of the matched text. `.length()` gives
  the length; `.first` and `.second` are the begin/end
  iterators.

- The character class `[^:]+` means "one or more characters
  that aren't a colon". This handles hostnames containing
  dots, dashes, etc. -- exactly what real hostnames look like.

- The `+` (one-or-more) on both groups ensures the strings
  aren't empty. `"foo:bar"` matches; `":bar"` and `"foo:"` do
  not.

- For the failure case, the function returns false without
  modifying `host` or `port`. The standard's out-param contract
  is "may have any value on failure"; tests don't probe the
  unchanged-on-failure invariant.

- Tests for failure include: no colon (`"localhost"`), no port
  (`"localhost:"`), no host (`":8080"`), non-digit port
  (`"localhost:abc"`), extra text after (`"localhost:8080 x"`).

- The regex doesn't validate port range (1..65535). For "port
  is in range", check the parsed int after extraction. Out of
  scope for this task.
