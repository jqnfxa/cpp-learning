# 58 — Variadic template recursion

> Spec: <https://en.cppreference.com/cpp/language/parameter_pack>

## What you learn

The init-list trick (step 55) is fine when each pack element is
processed independently. Some operations need **inter-element
state** — joining strings with a separator that doesn't appear at
the ends, computing a running maximum that compares to its
neighbors, etc. For those, the C++11 idiom is **head/tail
recursion**:

```cpp
/* Base case: single element. */
template <class T>
std::string format(const T &x);

/* Recursive case: peel off the head, recurse on the tail. */
template <class T, class U, class... Rest>
std::string format(const T &head, const U &second, const Rest &...rest);
```

Two overloads. The compiler picks the recursive one when there are
at least two arguments and the base case when there's exactly one.
The recursion strips off arguments one at a time until only one
remains.

C++17 fold expressions (`(args + ...)`) make many of these
patterns one-liners. Until then, head/tail is how you do it.

## Why two arguments in the recursive case?

Compare two ways to write the recursion:

```cpp
/* Form A: single-arg base + (head + 1+ tail). */
template <class T> std::string f(const T &x) { ... }
template <class T, class U, class... Rest>
std::string f(const T &x, const U &y, const Rest &...rest) {
    return one(x) + sep + f(y, rest...);   /* always at least 2 args here */
}

/* Form B: zero-arg base + (1+ head + tail). */
std::string f() { return ""; }
template <class T, class... Rest>
std::string f(const T &x, const Rest &...rest) {
    return one(x) + (sizeof...(Rest) ? sep + f(rest...) : "");
}
```

Form A is cleaner because it doesn't need the `sizeof...` check
inside the body to suppress the trailing separator. The two-arg
parameter ensures the recursive case is only chosen when there's a
"next" element after `head`. The single-arg base case handles the
terminator without a separator.

## Task

In `solution.hpp`, implement two `format` overloads:

```cpp
template <class T>
std::string format(const T &x);

template <class T, class U, class... Rest>
std::string format(const T &x, const U &y, const Rest &...rest);
```

`format` joins all arguments into a single space-separated string.
The base case handles `format(x)` returning just `x`. The recursive
case handles `format(x, y, ...)` returning `x + " " + format(y, ...)`.

| Call                          | Returns           |
|-------------------------------|-------------------|
| `format(1)`                    | `"1"`             |
| `format(1, 2)`                 | `"1 2"`           |
| `format(1, 2, 3)`              | `"1 2 3"`         |
| `format("a", "b", "c")`        | `"a b c"`         |
| `format(1, "+", 2, "=", 3)`    | `"1 + 2 = 3"`     |
| `format(3.14)`                 | `"3.14"`          |

## Run

```sh
./verify.sh
```

## Hints

- Base case body:
  ```cpp
  template <class T>
  std::string format(const T &x)
  {
      std::ostringstream oss;
      oss << x;
      return oss.str();
  }
  ```

- Recursive case body:
  ```cpp
  template <class T, class U, class... Rest>
  std::string format(const T &x, const U &y, const Rest &...rest)
  {
      std::ostringstream oss;
      oss << x << ' ' << format(y, rest...);
      return oss.str();
  }
  ```
  The `format(y, rest...)` call peels off `x`, leaves `y` as the
  new head. If `rest` is empty, the recursive call resolves to the
  single-arg base case; otherwise it stays in the recursive
  overload.

- Don't try to write a `format()` (empty) base case. With two
  recursion bases (empty AND single-arg), the recursive call has
  to decide which to terminate at. The two-arg recursion shape
  avoids that.

- Include `<sstream>`, `<string>`.
