# 64 — trivial and standard-layout types

> Spec: <https://en.cppreference.com/cpp/named_req/TrivialType>,
> <https://en.cppreference.com/cpp/named_req/StandardLayoutType>

## What you learn

C++11 formalised two orthogonal properties that together cover
what older code informally called "POD":

| Property             | Roughly: "you can…"                                      | Trait                          |
|----------------------|----------------------------------------------------------|--------------------------------|
| **trivial**          | `memcpy` it, default-construct without observable effect | `std::is_trivial<T>`           |
| **standard-layout**  | Pass it through a C ABI; first-member address == object address | `std::is_standard_layout<T>` |
| **POD** (deprecated) | Both of the above                                        | `std::is_pod<T>` (removed in C++20) |

They're **independent**: it's easy to have one without the other.

### Trivial

A class is *trivial* if it has *trivial* default/copy/move
ctors/assignments and a *trivial* destructor — i.e., none of them
do anything beyond the default. Adding a user-defined default
constructor (even an empty `S() {}`) breaks triviality. So does a
non-static data member with an in-class initialiser (NSDMI),
because it makes the implicit default constructor non-trivial.

```cpp
struct trivial_pair    { int a; int b; };           /* trivial */
struct has_init        { int a = 0; int b; };       /* NSDMI -> not trivial */
struct has_user_ctor   { int a; has_user_ctor(){} };/* user ctor -> not trivial */
```

### Standard-layout

A class is *standard-layout* if its layout matches C: all non-
static data members live in **one** class along the inheritance
chain, access control is uniform (no mixed `public` / `private`
data members), no virtual functions, no virtual bases, and the
first non-static data member is not the same type as the class's
first base. The key practical guarantee:

```cpp
struct s { int a; int b; };
static_assert(offsetof(s, a) == 0, "first member at offset 0");
```

You can also take `reinterpret_cast<int *>(&obj)` and get `&obj.a`.

### How they diverge

- A class with a private and a public data member is *trivial*
  (defaulted special members are still trivial) but **not**
  standard-layout (mixed access).
- A class with a user-defined default constructor `S() = default;`
  is trivial *and* standard-layout — `= default` keeps triviality.
- A class with `S() {}` (empty user body) is **not** trivial but
  *is* standard-layout — the empty body still counts as user-
  provided.

The takeaway for this task: each property is its own bit, and the
traits let you assert what you're getting.

## Task

In `solution.hpp`, declare four types so the static_asserts in
`tests.cpp` pass:

- `trivial_pod` — both trivial and standard-layout. Two `int`
  members, defaulted everything.
- `trivial_not_layout` — trivial but **not** standard-layout. Use
  mixed access control (one private, one public data member) to
  break standard-layout while keeping triviality.
- `layout_not_trivial` — standard-layout but **not** trivial. Add
  a user-provided default constructor (`S() {}`) to break
  triviality without breaking standard layout.
- `neither` — neither trivial nor standard-layout. A `virtual`
  member function is enough to ruin both at once.

Skeleton:

```cpp
struct trivial_pod
{
    int a;
    int b;
};

struct trivial_not_layout
{
public:
    int a;

private:
    int b;
};

struct layout_not_trivial
{
    int a;
    int b;
    layout_not_trivial() {}  /* user-provided -> not trivial */
};

struct neither
{
    int a;
    virtual ~neither() {}    /* virtual -> not trivial AND not standard-layout */
};
```

## Run

```sh
./verify.sh
```

## Hints

- All the validation is `static_assert` at namespace scope on
  `std::is_trivial<T>` and `std::is_standard_layout<T>`. The
  smoke runtime test just exists so `ctest` has something to run.

- For `trivial_not_layout`: mixed access on **data** members is
  what breaks standard-layout. Mixed access on **member functions**
  doesn't.

- For `layout_not_trivial`: `S() = default;` keeps triviality, so
  the empty-body form `S() {}` is what you want here.

- The compiler is allowed to use `is_trivial = false` whenever
  *any* special member is non-trivial — it's a strict definition.
  In practice, "no user-written special members and no NSDMIs and
  no virtual" gets you trivial.

- `std::is_pod<T>` exists in C++11 and is exactly
  `is_trivial && is_standard_layout`. It was deprecated in C++17
  and removed in C++20 because the two-axis view is more useful.
