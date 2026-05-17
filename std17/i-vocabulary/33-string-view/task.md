# 33 — `std::string_view`

> Spec: <https://en.cppreference.com/cpp/string/basic_string_view>

A non-owning view of a char range. Cheaper than
`const std::string&` for read-only parameters; accepts
literals, std::string, and char arrays uniformly without
allocation.

```cpp
void f(std::string_view sv);

f("literal");            /* no string allocation */
f(std::string("hello")); /* no copy */
f(my_buffer);            /* a char[] also works */
```

## Hints

- `string_view` is a pointer + length, no ownership.
- DOES NOT own; if the underlying string is destroyed, the
  view dangles. Don't store views past the source's
  lifetime.
- Has the same API as std::string for read-only operations
  (find, substr, etc.).
