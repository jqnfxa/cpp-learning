# 1 — Structured bindings: arrays

> Spec: <https://en.cppreference.com/cpp/language/structured_binding>

## What you learn

C++17 introduced **structured bindings**: a declaration form
that destructures an aggregate or tuple-like type into named
identifiers. The simplest form binds an array's elements:

```cpp
int arr[3] = {10, 20, 30};
auto [a, b, c] = arr;       /* a=10, b=20, c=30 */
```

Three structured-binding cases (this step covers the first):

1. **Arrays**: bind one identifier per element (number must
   match exactly).
2. **Tuple-like** (`std::tuple`, `std::pair`, `std::array`):
   bind via `std::tuple_size` + `std::get` (step 2).
3. **Struct/class with public members**: bind via memberwise
   access (step 3).

### The array form

The number of identifiers MUST match the array size:

```cpp
int arr[3] = {1, 2, 3};
auto [a, b]    = arr;       /* error: arity mismatch (3 vs 2) */
auto [a, b, c] = arr;       /* OK */
auto [a, b, c, d] = arr;    /* error: too many */
```

Each identifier is a **copy** of the corresponding element
(when using `auto`):

```cpp
int arr[3] = {1, 2, 3};
auto [a, b, c] = arr;
a = 99;                /* doesn't affect arr[0] -- copy */
```

For reference binding (`auto&` or `const auto&`), the
identifiers alias the array elements:

```cpp
int arr[3] = {1, 2, 3};
auto& [a, b, c] = arr;
a = 99;                /* arr[0] is now 99 */
```

### Const propagation

`const auto&` makes each identifier `const T&`:

```cpp
int arr[3] = {1, 2, 3};
const auto& [a, b, c] = arr;
a = 99;                /* error: a is const int& */
```

For mutable read-only views, this is the cleanest form.

### Works on any array kind

```cpp
int arr_c[3];                  /* C array */
std::array<int, 3> arr_std;    /* std::array (tuple-like, step 2) */

auto [a, b, c] = arr_c;        /* C array via array-form */
auto [a, b, c] = arr_std;      /* std::array via tuple-like form */
```

The C array form is the one this step uses. The std::array
form is technically the tuple-like form (step 2), since
std::array specializes `tuple_size` and `get`.

### Multi-dimensional arrays

Structured binding works one level deep:

```cpp
int matrix[3][2] = {{1,2}, {3,4}, {5,6}};
auto [row1, row2, row3] = matrix;     /* each row is int[2] */
auto [a, b] = row1;                    /* deeper bind */
```

You can't bind directly to `auto [a, b, c, d, e, f]` --
arrays only destructure their first dimension. Each row is
a separate array.

### The pre-C++17 alternative

Before structured bindings, you'd write:

```cpp
int arr[3] = {1, 2, 3};
int a = arr[0];
int b = arr[1];
int c = arr[2];
/* or */
int* p = arr;
int a = p[0], b = p[1], c = p[2];
```

Both work, but neither establishes the "destructure" intent
visually. Structured bindings put the per-element identifiers
right next to the source.

### Performance

Structured bindings are compile-time syntactic sugar -- no
runtime overhead. The compiler emits the same code as if you
wrote out the per-element assignments. With `auto`, each
identifier is a copy; with `auto&`, references; with `auto&&`,
forwarding (rare).

### Limits

- Number of identifiers must match the array's static size.
- The source must be a complete-type array (not a pointer).
  `int* p` with `auto [a, b] = p` is ill-formed.
- All identifiers have the same `auto` qualifier (you can't
  mix `int& a` and `int b`).

### Naming the bindings

The identifiers in `auto [a, b, c] = arr;` are like
declared variables, but bound to the array's elements. You
can use any valid identifier name; they're scoped to the
enclosing block.

### Step 2/3 preview

The next two steps cover the other structured-binding
forms:

- Step 2: tuples and pairs (`auto [k, v] = *map.begin();`)
- Step 3: structs (`auto [x, y] = point;`)

The array form here is the simplest -- direct positional
binding with arity exactly equal to the array's static
length.

## Task

In `solution.hpp`, fill in three functions that use
structured bindings on `int` arrays:

1. `destructure(arr)`: takes `const int (&)[3]`, returns a
   `std::tuple<int, int, int>` of the elements.

2. `destructure5(arr)`: takes `const int (&)[5]`, returns a
   `std::tuple<int, int, int, int, int>`.

3. `sum3(arr)`: takes `const int (&)[3]`, returns the sum of
   the three elements (destructured via structured binding).

```cpp
inline std::tuple<int, int, int> destructure(const int (&arr)[3]) {
    auto [a, b, c] = arr;
    return std::make_tuple(a, b, c);
}

inline int sum3(const int (&arr)[3]) {
    auto [a, b, c] = arr;
    return a + b + c;
}
```

## Run

```sh
./verify.sh
```

## Hints

- The CMakeLists sets `CMAKE_CXX_STANDARD 17` for this step
  and all of std17/. Structured bindings require C++17.

- The signature `const int (&arr)[3]` is a reference to a
  fixed-size 3-element array. Don't write `int* arr` --
  arrays decay to pointers and structured bindings don't
  work on pointers.

- The number of bindings must match the array size exactly.
  `auto [a, b]` on `int[3]` is ill-formed.

- `auto [a, b, c] = arr;` copies each element. Modifying `a`
  doesn't affect `arr[0]`. Use `auto& [a, b, c] = arr;` for
  aliasing.

- `const auto& [a, b, c] = arr;` makes each binding a
  `const int&` -- useful when you only need to read.

- The test `binding_reference_aliases` exercises the
  reference form: `auto& [a, b, c] = arr;` lets writes to
  a/b/c flow back into arr.

- All three functions can be written with the same pattern:
  bind, then either tuple-construct or compute.

- Tests verify: 3-element destructure (positive, negative,
  zero), 5-element destructure (matching values, sequential
  sum), sum3 across positive/large/cancelling values, direct
  array binding (no helper function), and the reference-
  alias form for writes.

- For std::array, the same destructuring works -- but
  through the tuple-like protocol (step 2 covers this in
  detail). C arrays use the dedicated array-form rule.
