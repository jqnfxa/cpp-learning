# 14 — `std::integer_sequence` and `std::index_sequence`

> Spec: <https://en.cppreference.com/cpp/utility/integer_sequence>

## What you learn

C++14 added a small compile-time-list utility for use in
template metaprogramming:

```cpp
template <class T, T... Ints>
struct integer_sequence;       /* (1) general form */

template <std::size_t... Ints>
using index_sequence = std::integer_sequence<std::size_t, Ints...>;   /* (2) common case */

template <std::size_t N>
using make_index_sequence = ...;                                       /* (3) builder */

template <class... T>
using index_sequence_for = std::make_index_sequence<sizeof...(T)>;     /* (4) for a pack */
```

It's a **carrier for a list of compile-time integers** --
the indices `0, 1, ..., N-1` typically. The key trick: you
deduce the indices into a function template's parameter pack
via overload resolution.

### The canonical use: unpacking a tuple

To call a function with a tuple's elements as separate args,
you need to write `std::get<0>(t), std::get<1>(t), ...,
std::get<N-1>(t)` at compile time. C++14's `index_sequence`
does this via pack expansion:

```cpp
template <class F, class Tuple, std::size_t... Is>
auto apply_impl(F &&f, Tuple &&t, std::index_sequence<Is...>)
{
    return std::forward<F>(f)(std::get<Is>(std::forward<Tuple>(t))...);
}

template <class F, class Tuple>
auto apply(F &&f, Tuple &&t)
{
    constexpr auto N = std::tuple_size<std::decay_t<Tuple>>::value;
    return apply_impl(
        std::forward<F>(f),
        std::forward<Tuple>(t),
        std::make_index_sequence<N>{});
}
```

The mechanism:

1. `make_index_sequence<3>` produces `index_sequence<0, 1, 2>`.
2. Passing that to `apply_impl` deduces `Is... = 0, 1, 2`.
3. The pack expansion `std::get<Is>(t)...` expands to
   `std::get<0>(t), std::get<1>(t), std::get<2>(t)`.
4. The function is called with those as separate args.

C++17 standardized this pattern as `std::apply`. For C++14,
you write the helper yourself.

### `make_index_sequence` and `index_sequence_for`

Two builders:

```cpp
std::make_index_sequence<5>     /* index_sequence<0, 1, 2, 3, 4> */
std::index_sequence_for<A, B, C> /* index_sequence<0, 1, 2> -- one per pack element */
```

`index_sequence_for<Ts...>` is convenience -- exactly
equivalent to `make_index_sequence<sizeof...(Ts)>`. Use it
when you have a parameter pack and need an index for each
element.

### The general form: `integer_sequence<T, ...>`

For non-`size_t` integers (e.g., compile-time values in a
specific type), the general template is available:

```cpp
std::integer_sequence<int, 1, -2, 3, -4>     /* compile-time list of ints */
```

You'd use this for things like a bit-mask metaprogram or a
factorial table. For the common "indices into a pack" case,
`index_sequence` is the right form.

### `sizeof...` and pack manipulations

The companion operator `sizeof...(pack)` returns the number
of elements in a parameter pack, at compile time:

```cpp
template <class... Ts>
constexpr auto count = sizeof...(Ts);   /* number of types */
```

Together with `index_sequence_for<Ts...>`, this lets you
write metaprograms that operate on each element of a pack
by its index.

### Fold expressions (C++17) replace the dummy-array trick

C++14 lacks fold expressions, so to "do something for each
expanded element," you need a workaround. The classic is the
**dummy array** trick:

```cpp
template <class... Args>
void each_print(Args &&... args)
{
    using expand = int[];
    (void)expand{0, ((void)(std::cout << args), 0)...};
}
```

The `0`-initialiser at the start ensures the array has at
least one element (so `int[]{}` of size zero isn't formed
when args is empty). The `(expression, 0)` comma trick
evaluates the expression and yields 0 for the array element.

C++17 fold expressions replace this with:

```cpp
((std::cout << args), ...);     /* C++17 fold over comma operator */
```

For C++14 you live with the dummy-array trick or recursive
template instantiation.

### Recursive vs index_sequence

The "classic" C++03/11 way to walk a parameter pack was
recursive template instantiation:

```cpp
template <class T>
T sum(T x) { return x; }

template <class T, class... Rest>
T sum(T x, Rest... rest) { return x + sum(rest...); }
```

This works but generates N specialisations for an N-arg
call. `index_sequence` enables a single non-recursive
expansion -- one template, N expanded references.

Compile time and code size both win with the
`index_sequence` approach for large packs.

### Real-world uses

- Implementing `std::apply` (C++17 standardised; C++14
  rolled by hand).
- Calling a constructor with arguments unpacked from a
  tuple.
- Building per-index lookups across heterogeneous data.
- Concatenating tuples by reindexing into their elements.
- Compile-time strings: `make_index_sequence<sizeof(s)-1>`
  to walk a string literal character-by-character.

### Performance

`index_sequence` itself has no runtime cost -- it's a
compile-time list with no storage. Constructed objects are
zero bytes. The "performance" win is that the expansion
happens in ONE non-recursive frame instead of N recursive
ones.

## Task

In `solution.hpp`, implement two compile-time-list helpers:

1. `sum_impl(t, index_sequence<Is...>)`: returns the sum of
   `std::get<Is>(t)...`.
2. `product_impl(t, index_sequence<Is...>)`: returns the
   product.

Use the dummy-array trick (C++14 has no fold expressions):

```cpp
int result = 0;
int dummy[] = {0, (result += std::get<Is>(t), 0)...};
(void)dummy;
return result;
```

The user-facing `apply_sum` and `apply_product` are
already wired up to call these via
`std::index_sequence_for<Ts...>`.

```cpp
template <class... Ts>
int apply_sum(std::tuple<Ts...> const &t)
{
    return detail::sum_impl(t, std::index_sequence_for<Ts...>{});
}
```

`sum_first_n<N>(t)` is wired up via
`std::make_index_sequence<N>{}` directly -- another way to
invoke the same helper.

## Run

```sh
./verify.sh
```

## Hints

- The dummy-array trick:
  ```cpp
  int dummy[] = {0, (result += std::get<Is>(t), 0)...};
  ```
  - The leading `0` ensures the array is non-empty even when
    `sizeof...(Is) == 0` (empty pack).
  - The pack expansion `(result += std::get<Is>(t), 0)...`
    evaluates `result += std::get<I>(t)` for each I and uses
    the comma operator to yield `0` (the array's element
    type).
  - `(void)dummy` silences "unused variable" warnings.

- Empty pack handling: when `Is...` is empty, the array has
  just the leading `0`, the loop body never runs, and
  `result` keeps its initial value (0 for sum, 1 for
  product). The "empty tuple" tests pin this.

- C++17 simplification: `int result = 0; ((result +=
  std::get<Is>(t)), ...); return result;`. The dummy array
  is the C++14 equivalent of that fold.

- `std::index_sequence_for<Ts...>` is a convenience alias.
  `std::make_index_sequence<sizeof...(Ts)>` does the same
  thing.

- `sum_first_n<3>(tuple)` calls the helper with
  `make_index_sequence<3>` -- the index pack is `0, 1, 2`,
  regardless of the tuple's total size.

- Tests verify: sum of various tuple sizes, sum with
  negatives, product including zero (sanity for the
  `*=` operator), empty-tuple identity values (sanity, not
  stub detectors), and `sum_first_n<N>` for several N.

- The pattern is broadly useful: ANY operation that needs to
  produce "f(elem_0), f(elem_1), ..." over a parameter pack
  can use index_sequence as the bridge.

- C++17's `std::apply(f, tuple)` is the production form for
  "call f with tuple's elements." C++14 doesn't have it; you
  implement it via index_sequence as shown.
