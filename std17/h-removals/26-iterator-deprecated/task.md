# 26 — Deprecated: `std::iterator`

C++17 deprecated `std::iterator<Category, T, Distance,
Pointer, Reference>` -- the base template you used to
inherit from to "tag" an iterator.

Modern form: declare the five typedefs directly:

```cpp
class my_iter {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = T;
    using difference_type   = std::ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;
    /* ... */
};
```

`std::iterator_traits<my_iter>` picks up these typedefs
automatically.

## Hints

- The five typedefs are what `std::iterator_traits` looks
  for.

- Without them, STL algorithms (`std::distance`,
  `std::find`, etc.) can't use your iterator.
