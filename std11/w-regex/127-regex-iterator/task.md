# 127 — `std::regex_iterator`

> Spec: <https://en.cppreference.com/cpp/regex/regex_iterator>

## What you learn

`std::regex_iterator` (and its typedef `std::sregex_iterator`
for `std::string`) is the **iterator over every match** of a
regex in a string. Where `regex_search` finds only the first
match, the iterator walks all of them.

```cpp
std::string text = "the quick brown fox";
std::regex word(R"(\w+)");

for (auto it = std::sregex_iterator(text.begin(), text.end(), word);
     it != std::sregex_iterator();
     ++it)
{
    std::cout << it->str() << "\n";
}
/* the
   quick
   brown
   fox */
```

The iterator is a **forward iterator** that yields
`std::smatch` results, one per match. The default-constructed
sentinel `std::sregex_iterator()` is the end value.

### The shape

```cpp
auto it = std::sregex_iterator(begin, end, regex);
```

Constructor takes:
- `begin`, `end` -- iterators into the source string.
- `regex` -- the compiled pattern.

`*it` is a `std::smatch` (you can do `it->str()`, `it->size()`,
etc.). `++it` advances to the next match.

### Reading captures across all matches

`regex_iterator` works with multiple capture groups too. Each
`*it` is a fresh `smatch` with `[0]` for the full match and
`[i]` for the captures.

```cpp
std::regex kv(R"((\w+)=(\w+))");
for (auto it = std::sregex_iterator(s.begin(), s.end(), kv);
     it != std::sregex_iterator(); ++it)
{
    std::string k = (*it)[1].str();
    std::string v = (*it)[2].str();
    /* ... */
}
```

The two captures fill `(*it)[1]` and `(*it)[2]` per match.

### Tokenisation pattern

The textbook use is tokenisation: pattern matches the
*tokens*, the iterator walks them. For "words", the pattern is
`\w+` (one-or-more "word characters" -- letters, digits,
underscore in ECMAScript).

```cpp
std::vector<std::string> tokenize_words(const std::string &s)
{
    static const std::regex word(R"(\w+)");
    std::vector<std::string> out;
    for (auto it = std::sregex_iterator(s.begin(), s.end(), word);
         it != std::sregex_iterator();
         ++it)
    {
        out.push_back(it->str());
    }
    return out;
}
```

This skips whitespace and punctuation -- whatever isn't a
word character is treated as a separator. The output is the
list of word substrings in order.

### `regex_token_iterator` — the "split" variant

There's also `std::regex_token_iterator`, which can yield
either the matches or the *gaps* (substrings between matches):

```cpp
std::regex sep(R"(\s+)");
auto it = std::sregex_token_iterator(s.begin(), s.end(), sep, -1);
/* yields substrings BETWEEN whitespace runs -- a Python-like split */
```

The `-1` argument means "the bits not matched by the pattern".
For "tokenize words", the direct `regex_iterator` form above is
more natural; `token_iterator` shines for "split on this
separator regex".

### Performance

The iterator is **lazy**: each `++it` triggers another
`regex_search` from the previous end. For large inputs with
many matches, allocate the vector with `reserve()` if you have
a rough size estimate.

The regex itself should be `static const` for the same reason
as in step 125 -- compile once, reuse forever.

## Task

In `solution.hpp`, implement one function:

```cpp
namespace task127
{
    /* Return the words (runs of \w characters) in s, in order. */
    std::vector<std::string> tokenize_words(const std::string &s);
}
```

## Run

```sh
./verify.sh
```

## Hints

- The body:
  ```cpp
  static const std::regex word(R"(\w+)");
  std::vector<std::string> out;
  for (auto it = std::sregex_iterator(s.begin(), s.end(), word);
       it != std::sregex_iterator();
       ++it)
  {
      out.push_back(it->str());
  }
  return out;
  ```

- `std::sregex_iterator` is `std::regex_iterator<std::string::const_iterator>`.
  It's the same type the tests use; no extra adapter needed.

- `it->str()` returns the matched substring as `std::string`.
  Equivalent to `(*it).str()`.

- `\w` in ECMAScript matches `[A-Za-z0-9_]`. Hyphens, dots,
  and most punctuation are NOT word characters -- they act
  as separators.

- The default-constructed `std::sregex_iterator()` is the
  end sentinel. Comparing against it terminates the loop.

- Empty input → empty vector. The iterator construction with
  `begin == end` returns the end sentinel immediately.

- Numbers count as words: `"abc 123"` → `["abc", "123"]`.
  `\w` matches digits.

- Mixed alphanumerics in a single token: `"id42 v3"` →
  `["id42", "v3"]`. `\w+` is greedy.

- Tests verify exact vector contents (order matters), word
  count for various inputs, handling of multiple spaces /
  tabs / newlines / punctuation, and empty input.

- C++20 added `std::ranges` regex iterators that integrate
  with the ranges library. For C++11 this is the idiomatic
  form.
