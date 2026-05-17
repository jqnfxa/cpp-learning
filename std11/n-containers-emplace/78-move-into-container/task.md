# 78 — move into a container

> Spec: <https://en.cppreference.com/cpp/container/vector/push_back>

## What you learn

`std::vector::push_back` has two overloads in C++11:

```cpp
void push_back(const T &value);   /* (1) copy */
void push_back(T &&value);        /* (2) move */
```

When you call `v.push_back(s)` with a non-const lvalue `s`, the
copy overload binds. When you call `v.push_back(std::move(s))`,
the rvalue-reference overload binds and the element is
*move-constructed* from `s`. Same for `emplace_back` when its
single argument is the element type itself.

For `std::string` (heap-allocated when long enough) the
difference matters concretely:

- **Copy**: allocates a new buffer, copies all the characters.
- **Move**: steals the existing heap buffer. Constant time. The
  source string is left in a *valid but unspecified state* —
  in practice both `libstdc++` and `libc++` leave it empty when
  the source was heap-allocated.

```cpp
std::string s = "this is a string that is too long for SSO";

std::vector<std::string> v;
v.push_back(std::move(s));
/* v[0] now owns the original heap buffer.
   s is in a moved-from state; reading it is fine, but its
   value is implementation-defined. In practice: empty. */
```

### Why this matters

In a hot loop building a vector of strings, copying is the
silent expense:

```cpp
std::string line;
while (read_line(line))
{
    v.push_back(line);              /* copy every iteration */
}
```

vs.

```cpp
std::string line;
while (read_line(line))
{
    v.push_back(std::move(line));   /* move every iteration */
    line.clear();                    /* explicit reset for clarity */
}
```

The second form avoids the per-iteration `O(line.size())` copy.
For large strings or tight loops, that's the entire performance
difference between "fast" and "slow".

### The "moved-from" contract

> The C++ standard guarantees that a moved-from object is in a
> *valid* state — you can destroy it, assign to it, or call any
> non-precondition member function. It does NOT guarantee what
> the contents are.

For `std::string` specifically, the practical behaviour from the
two major standard libraries:

- Source used heap storage (long string): **emptied**
  (`size() == 0`, `data()` points at the small buffer or some
  canonical empty representation).
- Source used SSO (short string): the move was essentially a
  copy; the source typically remains unchanged.

For this task we use a *long* string so the empty-after-move
behaviour is observable.

### `std::move` is a cast, not a verb

`std::move(x)` doesn't *do* anything at runtime — it's
`static_cast<T &&>(x)`. The actual moving happens inside the
function you hand the result to. If that function has no
rvalue-aware overload, `std::move(x)` falls back to the
const-lvalue-reference overload and you get a copy.

For containers, the rvalue overload exists, so
`push_back(std::move(s))` is genuinely a move.

## Task

In `solution.hpp`, implement two functions:

```cpp
namespace task78
{
    /* v.push_back(s).  Copies s; s is unchanged after the call. */
    void push_copy(std::vector<std::string> &v, std::string &s);

    /* v.push_back(std::move(s)).  Moves s; s is left empty
     * (libstdc++/libc++ behaviour for heap-allocated strings). */
    void push_move(std::vector<std::string> &v, std::string &s);
}
```

The functions take `std::string &` by non-const reference so
the tests can observe the effect on `s` after the call.

## Run

```sh
./verify.sh
```

## Hints

- Both implementations are one line each. `push_copy` is
  `v.push_back(s);`. `push_move` is `v.push_back(std::move(s));`.

- The tests use long strings (> 32 chars) to bust SSO. With SSO
  active, "moved-from is empty" wouldn't hold portably and the
  test would be testing an implementation choice. Long strings
  are heap-allocated and reliably emptied by move.

- The test for `push_move` also captures `s.data()` before the
  call and asserts `v[0].data()` equals the captured pointer
  after. That's the strongest proof a move actually happened —
  the heap buffer is now owned by the vector element, not by
  any new allocation.

- A common mistake: `auto v = std::vector<std::string>{}; v.push_back(s); s.clear();`
  expecting "moving" the string. That's a copy followed by a
  clear -- two operations, no move. `std::move(s)` is the cast
  that selects the rvalue overload.

- Note the asymmetry with `emplace_back`: `emplace_back(s)`
  also makes a copy (the rvalue overload sees the lvalue and
  drops to copy), `emplace_back(std::move(s))` makes a move.
  `std::move` is what selects the rvalue path.

- For your own types, the same principle applies: make the move
  constructor available (and noexcept where possible), then
  call `std::move` whenever you have a named local you no
  longer need.
