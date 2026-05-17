# 91 — `std::uninitialized_copy_n`

> Spec: <https://en.cppreference.com/cpp/memory/uninitialized_copy_n>

## What you learn

The "uninitialized_*" family in `<memory>` operates on **raw,
unconstructed memory**. They're the low-level primitives that
container implementations (like `std::vector::push_back`'s
reallocation path) use to build the next buffer.

```cpp
template <class It, class Size, class P>
P uninitialized_copy_n(It first, Size n, P d_first);
```

The contract:
- `[d_first, d_first + n)` is a raw, uninitialised range — bytes
  with the right size and alignment for `T`, but no live `T`
  objects in them.
- `[first, first + n)` is a valid source range of `T`.

The algorithm **copy-constructs** each destination element from
the corresponding source element, using *placement new*:

```cpp
new (static_cast<void *>(addressof(*d_first))) T(*first);
```

It's *not* `*d_first++ = *first++`. That would be assignment,
which requires the destination to already hold a live `T` (so
the assignment operator has something to assign *to*). Raw
memory has no such thing.

### When you need this

You almost never call it directly. It's mostly an internal
primitive. But you encounter it when:

- Implementing a container with manual memory management.
- Writing a memory pool / arena.
- Implementing a small-buffer optimisation.

The two-pass pattern in `std::vector`:

```cpp
/* On grow: */
void *raw = ::operator new(new_cap * sizeof(T));
T *new_data = static_cast<T *>(raw);
std::uninitialized_copy_n(begin(), size(), new_data); /* construct *//* ...destroy old elements, free old buffer, swap pointers... */
```

Knowing this layer means you understand what `vector::reserve`
does and why it has the "no copies if no growth" guarantee.

### Exception safety

If the i-th copy-construction throws, the algorithm destroys
all the previously-constructed elements (i.e. those at
`d_first` ... `d_first + i - 1`) before propagating the
exception. The contract: the destination is left in the same
state as before the call (no live `T` objects in it). The
caller still owns the *raw* memory; the algorithm only worries
about the `T` lifetimes.

### Pairing with explicit destruction

After you use `uninitialized_copy_n` to *construct* `T`s in
raw memory, **you are responsible for destroying them** before
freeing the storage. The destructor doesn't run automatically;
the raw buffer is just bytes.

```cpp
for (std::size_t i = 0; i < n; ++i) p[i].~T();
::operator delete(raw);
```

Forgetting the destructor calls leaks any heap data the `T`s
own (strings, vectors, etc.). The pattern is unavoidable when
you're operating at this layer.

## Task

In `solution.hpp`, implement one function that demonstrates the
full uninitialized-storage round-trip:

```cpp
namespace task91
{
    /* Steps:
     *   1. Allocate raw storage for `count` std::strings via
     *      ::operator new(...).
     *   2. Copy-construct the first `count` strings of src into that
     *      raw storage using std::uninitialized_copy_n.
     *   3. Build a std::vector<std::string> from the constructed
     *      strings.
     *   4. Destroy the constructed strings explicitly
     *      (p[i].~basic_string()).
     *   5. Deallocate the raw storage with ::operator delete.
     *   6. Return the vector.
     *
     * Precondition: count <= src.size().
     */
    std::vector<std::string>
    via_uninitialized_copy_n(const std::vector<std::string> &src,
                             std::size_t count);
}
```

## Run

```sh
./verify.sh
```

## Hints

- Allocate via `::operator new(count * sizeof(std::string))`.
  This gives you raw bytes -- a `void *` that you cast to
  `std::string *`. Don't use `new std::string[count]` -- that
  default-constructs `count` strings, which is the opposite of
  what you want.

- `std::uninitialized_copy_n(src.begin(), count, p);` does the
  copy-construction work. The destination is the `std::string *`
  you got from the cast.

- After populating the raw buffer with valid strings, the
  natural way to build the vector is the range constructor:
  `std::vector<std::string> out(p, p + count);`. This copies
  the constructed strings into the vector's own storage.

- Explicit destruction:
  ```cpp
  for (std::size_t i = 0; i < count; ++i) p[i].~basic_string();
  ```
  Use `basic_string` (the template), not `string` (the typedef),
  for portability of the destructor syntax. Both work on
  libstdc++/libc++.

- `::operator delete(raw)` -- the global form. Match the
  `::operator new` you used.

- `count == 0` is the edge case to think about: `operator new(0)`
  is allowed and returns a valid (possibly null, possibly
  unique) pointer; matching `operator delete` is required.
  `uninitialized_copy_n(..., 0, ...)` is a no-op. The
  loop-over-zero-elements destructor body is also a no-op. So
  count==0 is automatically correct if you follow the steps.

- A `try/catch` around `uninitialized_copy_n` to handle
  exceptions would be the textbook-complete version. For this
  task we trust `std::string`'s short-string optimisation and
  small allocations not to throw mid-construction. Real
  container code adds the catch + cleanup.

- This is the lowest-level container-implementation primitive
  you'll touch in this curriculum. Everything else is built on
  top of it. Once you've written this once, container internals
  are demystified.
