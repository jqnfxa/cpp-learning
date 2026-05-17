# 23 — `std::get<T>(tuple)` (by type)

> Spec: <https://en.cppreference.com/cpp/utility/tuple/get>

## What you learn

C++14 added a by-type overload of `std::get` for tuples:

```cpp
auto t = std::make_tuple(42, std::string("alice"), 3.14);

auto age   = std::get<int>(t);             /* 42 */
auto name  = std::get<std::string>(t);     /* "alice" */
auto score = std::get<double>(t);          /* 3.14 */
```

C++11 only had the by-index form:

```cpp
auto age   = std::get<0>(t);
auto name  = std::get<1>(t);
auto score = std::get<2>(t);
```

The by-type form is more self-documenting AND robust to
field reordering. If someone refactors the tuple type from
`<int, string, double>` to `<string, double, int>`, the
by-index code silently picks up the wrong field; the by-type
code keeps working.

### The signature

```cpp
template <class T, class... Types>
T &get(std::tuple<Types...> &t) noexcept;

template <class T, class... Types>
const T &get(const std::tuple<Types...> &t) noexcept;

template <class T, class... Types>
T &&get(std::tuple<Types...> &&t) noexcept;
```

Three overloads:
- Mutable lvalue tuple -> `T &` (writable reference).
- Const tuple -> `const T &`.
- Rvalue tuple -> `T &&`.

### Constraint: T must occur exactly once

If `T` doesn't appear in the tuple's element types, the call
is ill-formed. If `T` appears MORE than once (the tuple has
two `int` elements, you `get<int>(t)`), the call is **also**
ill-formed -- ambiguity rejection at compile time:

```cpp
auto t = std::make_tuple(1, 2, "three");  /* int, int, const char* */
std::get<int>(t);   /* error: int appears twice */
std::get<0>(t);     /* OK: by index */
```

For tuples with duplicate types, you must use the by-index
form. The by-type form is designed for the common case where
each field has a unique type.

### Modifying through `get<T>`

`std::get<T>` returns a REFERENCE -- you can write through
it:

```cpp
auto t = std::make_tuple(0, std::string(""), 0.0);
std::get<int>(t) = 99;
std::get<std::string>(t) = "hello";
std::get<double>(t) = 3.14;
```

Same semantics as the by-index form. Const tuples return
`const T &` (no writes).

### Use cases

**Self-documenting tuple destructuring** (before C++17
structured bindings):

```cpp
auto p = get_profile();
int age = std::get<int>(p);
std::string name = std::get<std::string>(p);
double score = std::get<double>(p);
```

Compare to the C++11 form:

```cpp
int age = std::get<0>(p);
std::string name = std::get<1>(p);
double score = std::get<2>(p);
```

The first reads as "the int field, the string field, the
double field." The second requires you to know the tuple's
order.

**Hetero arrays via tuples**:

```cpp
auto config = std::make_tuple(
    /*timeout=*/std::chrono::seconds(30),
    /*retries=*/3,
    /*host=*/std::string("localhost")
);

auto timeout = std::get<std::chrono::seconds>(config);
auto host    = std::get<std::string>(config);
auto retries = std::get<int>(config);
```

The tuple acts like a small heterogeneous container; access
by type is the natural read pattern.

### Combining with index access

```cpp
auto t = std::make_tuple(1, 2.0, "three");

auto a = std::get<0>(t);     /* by index: 1 */
auto b = std::get<double>(t);   /* by type: 2.0 */
```

You can mix the two forms freely in the same expression.
The compiler resolves each call independently.

### C++17 structured bindings

C++17 added a more concise destructuring form:

```cpp
auto [age, name, score] = get_profile();    /* C++17 */
```

This binds each element by index automatically. For C++14
code, `std::get<T>` is the closest you get to "destructure
by type."

### `tuple_element` and `tuple_size` (related but separate)

`std::tuple_element_t<I, Tuple>` -- the type at index I.
`std::tuple_size_v<Tuple>` -- the number of elements
(C++17 alias; in C++14 it's `tuple_size<Tuple>::value`).

These are the introspection counterparts of `get<I>` /
`get<T>`. Step 14 used `tuple_size` for its iterating
helper.

### `std::pair` interop

`std::pair<A, B>` is conceptually a 2-tuple. `std::get<T>(pair)`
works the same way:

```cpp
std::pair<int, std::string> p{42, "x"};
auto i = std::get<int>(p);          /* 42 */
auto s = std::get<std::string>(p);  /* "x" */
```

Both forms work for both tuple and pair (and array).

### Doesn't apply to `std::variant`

C++17 `std::variant<T...>` has its own `std::get<T>` /
`std::get<I>` family. The semantics differ: variant's
`std::get` returns the *active* alternative; tuple's
returns the field with that type. Don't confuse them; same
name, different concepts.

## Task

In `solution.hpp`, fill in three accessor functions to use
the C++14 by-type get:

```cpp
inline int get_age(const profile &p) {
    return std::get<int>(p);
}

inline std::string get_name(const profile &p) {
    return std::get<std::string>(p);
}

inline double get_score(const profile &p) {
    return std::get<double>(p);
}
```

The `profile` tuple is `std::tuple<int, std::string, double>`.

## Run

```sh
./verify.sh
```

## Hints

- `<tuple>` is the header. Already included.

- `std::get<T>(tuple)` works when `T` appears exactly once in
  the tuple. For our profile (int, string, double), each
  type appears once, so all three by-type calls are valid.

- For tuples with duplicate types (`tuple<int, int>`),
  `std::get<int>` is ill-formed -- use by-index instead.

- `std::get<int>(p)` returns by reference for non-const
  tuples (mutable) and by const-reference for const tuples.
  The "returns_correct_type" test pins
  `decltype(get<string>(p)) == string&`.

- `std::get<int>(p) = 99` writes to the tuple's int field.
  Useful for in-place tuple updates.

- The tests cover: each field value, direct get<T> calls,
  modification through get<T>, const tuple access, type
  preservation.

- The reverse mapping (get value at index I as T) is
  `std::get<I>(t)` -- by index. Both coexist.

- C++17 structured bindings (`auto [a, b, c] = t;`) make this
  even more concise. For C++14, `std::get<T>` is the most
  idiomatic accessor.

- For `std::pair`, `std::array`, and `std::tuple`, the
  `std::get<T>` mechanism is uniform. The same syntax works
  across all three.
