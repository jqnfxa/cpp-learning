# 130 — `std::scoped_allocator_adaptor`

> Spec: <https://en.cppreference.com/cpp/memory/scoped_allocator_adaptor>

## What you learn

`std::scoped_allocator_adaptor` is the C++11 utility that
**propagates an outer container's allocator down into its
inner containers**. Without it, every level of nesting picks
its own (default-constructed) allocator.

```cpp
using inner  = std::vector<int, A<int>>;
using outer  = std::vector<inner, std::scoped_allocator_adaptor<A<inner>>>;

outer m{ std::scoped_allocator_adaptor<A<inner>>(my_a) };
m.emplace_back();     /* the new inner gets `my_a` -- automatically */
m.back().push_back(0);
```

The inner `vector<int>` was constructed by the outer, and the
adaptor invisibly passed `my_a` (rebound to `A<int>`) as its
allocator. Without the adaptor, the inner uses
`A<int>{}` -- the *default* state, not the outer's state.

### The problem it solves

Imagine you have an arena allocator with a pointer to a slab
of memory. You feed it to the outer container so that the
outer's elements are arena-allocated. But the elements
*themselves* might be containers (a vector inside a vector,
a string inside a map). Those nested containers also need
storage -- and you'd like THEIR storage to come from the
same arena.

Without `scoped_allocator_adaptor`, you have to manually pass
the arena allocator to every constructor of every nested
container. That's both tedious and impossible for the
container's *internal* operations (resize, insert,
emplace_back) which construct elements from arguments
alone.

With `scoped_allocator_adaptor`, the outer's allocator is
propagated automatically through `std::uses_allocator`
construction.

### How propagation actually works

Three pieces cooperate:

1. **`std::uses_allocator<T, A>`**: a type trait that's
   `true` if `T` has a nested `allocator_type` and that type
   is convertible-from `A`. STL containers all have this.

2. **`std::scoped_allocator_adaptor::construct(T*, args...)`**:
   the adaptor's `construct` overload. Before forwarding to
   placement new, it checks `uses_allocator<T, inner_allocator_type>`.
   If true, it inserts the inner allocator into the constructor
   call.

3. **Allocator-extended constructors**: every STL container has
   a constructor like `vector(const A&)` and
   `vector(allocator_arg_t, const A&, ...)`. The adaptor uses
   one of these when constructing a nested element.

When the outer vector calls `emplace_back()`, it calls
`allocator_traits::construct(adapted_alloc, p)`. The adaptor's
`construct` notices `inner` uses an allocator and rewrites
the call to `inner(my_inner_alloc)`. The inner vector is
born with the right allocator.

### A picture

```cpp
/* Without scoped_allocator_adaptor */
outer = vector<inner, A<inner>>;     A<inner>(tag=7) supplied
m.emplace_back();                    /* construct inner() */
                                     /* inner allocator = A<int>() default = tag 0 */
m.back().push_back(0);               /* allocates with tag 0 */

/* With scoped_allocator_adaptor */
outer = vector<inner, scoped_allocator_adaptor<A<inner>>>;
m.emplace_back();                    /* construct inner(A<int>(7)) */
                                     /* inner allocator = tag 7 */
m.back().push_back(0);               /* allocates with tag 7 */
```

### Multi-level

`scoped_allocator_adaptor` is variadic:

```cpp
template <typename Outer, typename... Inners>
class scoped_allocator_adaptor;
```

`scoped_allocator_adaptor<A, B, C>` means "outer = A, level-1
inners = B, level-2+ inners = C (and recursively all deeper
levels)". For two-level nesting with the same allocator type
all the way down, the common form is:

```cpp
std::scoped_allocator_adaptor<A>
```

i.e., one template argument. The same allocator is propagated
through every level.

### When you DON'T need it

`scoped_allocator_adaptor` only matters for **stateful**
allocators where the state matters per-container. For:

- `std::allocator<T>` -- stateless, default is fine, scoped
  adaptor changes nothing observable.
- A counting allocator (step 129) -- stateless, no
  propagation needed.

It matters for:

- Arena / pool allocators with a specific memory region.
- PMR-style allocators (the `std::pmr::polymorphic_allocator`
  in C++17 is uses_allocator-aware and the canonical use case).
- Any allocator whose state (memory region, NUMA node, ID
  tag) you want to carry into nested containers.

### Construction

The simplest form: pass the inner allocator and the adaptor
wraps it.

```cpp
std::scoped_allocator_adaptor<A<inner>> a(my_inner_alloc);
outer m(a);
```

You can also construct it from individual allocators per level
(for the multi-arg adaptor), but for the single-level case
above the wrapping is direct.

## Task

The scaffold defines a tagged allocator that logs every
allocation along with its tag. It builds a 2-row matrix using
an outer-vector-of-inner-vectors.

`solution.hpp` has the OUTER allocator declared as plain
`tagged_alloc<inner>`. Switch it to
`std::scoped_allocator_adaptor<tagged_alloc<inner>>` so the
inner vectors inherit the outer's tag.

```cpp
namespace task130
{
    /* The TODO is on this line: */
    using outer_alloc = std::scoped_allocator_adaptor<tagged_alloc<inner>>;

    /* unchanged: */
    using outer = std::vector<inner, outer_alloc>;
}
```

## Run

```sh
./verify.sh
```

## Hints

- Just one line to change: the `using outer_alloc = ...`
  alias. Wrap the existing `tagged_alloc<inner>` in
  `std::scoped_allocator_adaptor<...>`.

- `#include <scoped_allocator>` is already in the scaffold.

- The function `matrix_alloc_tags(tag)` builds a 2-row matrix
  and returns the per-allocation tag log. With the scoped
  adaptor in place, all three entries should equal `tag`.
  Without it, entries 1 and 2 will be `0` (the default tag of
  a default-constructed inner allocator).

- `std::vector` is uses_allocator-aware (it has a nested
  `allocator_type` and an allocator-extended ctor). The
  adaptor does the propagation automatically; you don't write
  any code to forward the allocator yourself.

- `scoped_allocator_adaptor<A>` (one template argument)
  propagates `A` through every level of nesting. You'd reach
  for the multi-arg form `scoped_allocator_adaptor<Outer,
  Inner1, Inner2>` only if different levels need DIFFERENT
  allocator types -- a rare configuration.

- The "most vexing parse" trap: writing `outer m(A(tag))`
  declares a function, not a variable. The scaffold names the
  allocator `a` first, then constructs `m(a)`.

- This is the textbook reason `scoped_allocator_adaptor`
  exists: stateful allocators with nested containers. C++17
  added `std::pmr::polymorphic_allocator` which makes the
  same propagation more ergonomic (the `polymorphic_allocator`
  IS the adaptor, internally). For C++11, this is the
  canonical way.

- 8 runtime tests: log size = 3 (one outer + two inner),
  outer's tag is preserved, inner's tags inherit the outer's
  tag (the KEY tests), different tags propagate independently,
  all tags uniform per call, distinct calls don't bleed, tag
  zero is itself a valid tag, allocator equality compares
  tags.
