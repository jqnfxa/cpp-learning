# 3 — Structured bindings: structs

> Spec: <https://en.cppreference.com/cpp/language/structured_binding>

## What you learn

The third structured-binding form: **any class type with all
public non-static data members**. The bindings match the
data members in **declaration order**:

```cpp
struct point { int x; int y; };

point p{1, 2};
auto [a, b] = p;       /* a = p.x, b = p.y -- declaration order */
```

No `std::tuple_size` specialization needed -- the compiler
deduces the count by counting the public data members.

### When the struct form applies

The class must be:

- A class or struct with **all public non-static** data
  members.
- No anonymous union members at the same scope.
- Not a class that specializes `std::tuple_size` (in which
  case the tuple-like form kicks in -- step 2).

If the type meets these criteria, structured binding pulls
the data members directly. For mixed-access classes, you
need accessors or the tuple-like protocol.

### Declaration order matters

```cpp
struct point { int x; int y; };

auto [a, b] = point{1, 2};
/* a is x (1), b is y (2) -- in declaration order */

struct reversed { int y; int x; };
auto [a, b] = reversed{2, 1};
/* a is y (2), b is x (1) -- different declaration order */
```

If you reorder the struct's members, the meaning of `[a, b]`
changes silently. This is the SAME problem as `std::get<0>`
-- positional access. C++20 designated initializers don't
help with destructuring; the binding is by position.

### Reference forms

Same `auto` / `auto&` / `const auto&` qualifiers as before:

```cpp
point p{1, 2};

auto [a, b]       = p;     /* a, b are copies (int) */
auto& [a, b]      = p;     /* a, b alias p.x, p.y */
const auto& [a, b] = p;    /* a, b are const int& */
```

`auto& [a, b] = p;` and then `a = 99;` updates `p.x`. This
is the main reason to use the reference form -- in-place
field updates without naming the struct.

### Doesn't apply to private members

```cpp
class private_box {
    int hidden;
public:
    int public_v;
};

auto [a] = private_box{1, 2};       /* error: not all public */
```

For classes with mixed access, the structured binding form
doesn't help. You'd need accessors (getters) or specialize
the tuple-like protocol.

### Nested structs

You can bind one level deep; nested structs require nested
binding:

```cpp
struct line { point p1; point p2; };

line l{{1,2}, {3,4}};
auto [a, b] = l;       /* a, b are points */
auto [a1, a2] = a;     /* nested bind */
```

You can't write `auto [[x1, y1], [x2, y2]] = l;` -- the
binding is single-level.

### Reads vs writes

Structured-binding on `auto` creates LOCAL copies. Mutations
on those copies don't propagate to the source:

```cpp
point p{1, 2};
auto [a, b] = p;
a = 99;             /* p.x is still 1 */
```

For propagation, use `auto&`:

```cpp
point p{1, 2};
auto& [a, b] = p;
a = 99;             /* p.x is now 99 */
```

### Combination with `if`-init-statement (step 4)

```cpp
if (auto [a, b] = some_function(); a > 0) {
    /* use a, b */
}
```

The bindings live for the duration of the if/else clause.
Step 4 covers this.

### Combination with range-for

```cpp
std::vector<point> v;
for (auto const& [x, y] : v) {
    /* x, y are const int& for each element */
}
```

Same as for map iteration -- each iteration's element is
destructured before the body.

### vs std::tie

C++11's `std::tie` only works for tuple-like sources. For
structs, you'd manually unpack:

```cpp
int a, b;
{
    auto p = make_point();
    a = p.x;
    b = p.y;
}
```

C++17's struct form makes this implicit:

```cpp
auto [a, b] = make_point();
```

One line, names are explicit, source can be a temporary.

### Compiler-deduced count

The compiler counts the public non-static data members at
the binding site. If you write the wrong number of
identifiers, you get a compile error:

```cpp
point p;
auto [a] = p;          /* error: too few */
auto [a, b, c] = p;    /* error: too many */
auto [a, b] = p;       /* OK */
```

The arity must match the struct's member count exactly.

## Task

In `solution.hpp`, fill in four functions:

```cpp
inline int point_sum(point p) {
    auto [a, b] = p;
    return a + b;
}

inline point point_swap(point p) {
    auto [a, b] = p;
    return {b, a};
}

inline std::string profile_label(profile p) {
    auto [age, name, score] = p;
    (void)score;
    return name + ":" + std::to_string(age);
}

inline double profile_score(profile p) {
    auto [age, name, score] = p;
    (void)age;
    (void)name;
    return score;
}
```

The `point` struct has two `int` members; `profile` has
`int / std::string / double`. Both are aggregates.

## Run

```sh
./verify.sh
```

## Hints

- `auto [a, b] = p;` works because `point` is an aggregate
  with two public `int` members.

- `auto [age, name, score] = p;` works for `profile` because
  all three members are public.

- The names you choose for the bindings can be anything;
  they don't need to match the struct's field names. But
  conventionally they DO match for readability.

- For `point_swap`, return `{b, a}` -- a brace-init list
  that the return type `point` deduces from. The two ints
  fill in `x` and `y` in declaration order.

- For `profile_label`, the result format is `"name:age"`.
  Use `std::to_string(age)` for the int -> string
  conversion.

- The `direct_reference_binding` test exercises `auto& [a,
  b] = p;` which lets writes flow back into the struct.
  This is the C++17 way to update fields without naming
  them.

- The `binding_takes_declaration_order` test pins that
  `[a, b]` follows the struct's declaration order, not
  alphabetical or anything else. Reorder the struct and the
  semantics change.

- For mixed-access classes (some private, some public),
  the struct form doesn't apply. Use accessors or the
  tuple-like protocol.

- Combined with C++17's other features (range-for, if-init,
  inline variables), structured bindings make for code
  that's both compact and explicit.
