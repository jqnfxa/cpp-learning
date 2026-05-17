# 71 — `std::weak_ptr<T>`

> Spec: <https://en.cppreference.com/cpp/memory/weak_ptr>

## What you learn

`shared_ptr` reference counts are what they say on the tin: every
`shared_ptr` to an object adds one. That's a problem the moment
you draw two boxes with arrows in both directions:

```
  Parent ----shared----> Child
  Parent <---shared----- Child       /* the second arrow is the bug */
```

Each holds the other alive. When the outside world drops both
handles, neither count reaches zero. Both objects leak. Forever.

`std::weak_ptr<T>` is the fix. A `weak_ptr` **observes** a
`shared_ptr`-managed object without participating in its
ownership:

- It does **not** affect the strong (`use_count`) count.
- It can be queried for whether the object still exists
  (`expired()`).
- To use the object you `lock()` the `weak_ptr`, which returns a
  `shared_ptr<T>`. If the object is gone, `lock()` returns an
  empty `shared_ptr`.

```cpp
std::shared_ptr<int> s = std::make_shared<int>(42);
std::weak_ptr<int>   w = s;       /* s.use_count() still 1 */

if (auto sp = w.lock())            /* atomic "is it alive? then own it" */
{
    std::cout << *sp;              /* safe -- we own a strong ref now */
}

s.reset();                          /* drop the only strong ref */
w.expired();                        /* true */
w.lock();                           /* empty shared_ptr */
```

The two halves of the **control block** correspond to these two
counts. Strong count hits zero -> object destroyed. Weak count
(plus the "1 if strong > 0" accounting entry) hits zero -> the
control block itself can be freed.

### Why `lock()` and not just dereference

A bare "is it alive?" check followed by a dereference is racy:

```cpp
if (!w.expired())
{
    *w.lock();   /* expired() said no, but another thread could have
                    dropped the last shared_ptr in between */
}
```

`w.lock()` is the *atomic* "if it's still alive, give me a strong
reference to it, otherwise give me nothing". It's the one call
you should reach for. `expired()` exists for diagnostics.

### The canonical use: parent/child

```cpp
struct Child;

struct Parent
{
    std::vector<std::shared_ptr<Child>> children;  /* owns the children */
};

struct Child
{
    std::weak_ptr<Parent> parent;                   /* observes the parent */
};
```

The "owning" direction uses `shared_ptr`, the "back-reference"
direction uses `weak_ptr`. The cycle is broken because
`weak_ptr` doesn't count.

Other places `weak_ptr` shows up:

- **Caches** -- map keys to `weak_ptr<T>`; the entry becomes
  invalid when the last `shared_ptr` is dropped, so the cache
  doesn't keep dead objects alive.
- **Observers / event sinks** -- the subject holds `weak_ptr`s
  to its listeners so that a forgotten listener doesn't prevent
  cleanup.
- **`enable_shared_from_this`** (step 72) -- the base class
  stores a `weak_ptr` to `this` so a member can hand out a
  `shared_ptr` without creating a new control block.

## Task

In `solution.hpp`, fix the cycle between `Parent` and `Child`.
The provided code has the back-reference as a `shared_ptr` --
that's the bug. Change it to a `weak_ptr<Parent>` and update
`parent_name()` to `lock()` it before dereferencing.

```cpp
struct Parent { /* unchanged: owns children via shared_ptr */ };

struct Child
{
    std::string name;

    /* TODO: change shared_ptr<Parent> -> weak_ptr<Parent> */
    std::shared_ptr<Parent> parent;

    /* TODO: lock() the weak_ptr; "" if expired */
    std::string parent_name() const;
};
```

The instrumentation (`parents_alive()`, `children_alive()`)
exists so the tests can observe whether the cycle leaks both
objects -- the cleanest possible proof that `weak_ptr` is the
fix.

## Run

```sh
./verify.sh
```

## Hints

- The whole code change is two lines: the field type, and the
  body of `parent_name()`.

- `weak_ptr<T>` assigns from `shared_ptr<T>` directly --
  `c->parent = p;` works the same way it did with
  `shared_ptr<Parent>`. The `link(p, c)` helper doesn't need to
  change.

- Implement `parent_name()` as
  `if (auto sp = parent.lock()) return sp->name; return {};`. The
  one-line "lock-and-use" idiom.

- The test fixture's `SetUp` asserts both alive counters are 0.
  If they aren't, a previous test leaked -- which is exactly what
  the cycle bug does. With the stub, expect that downstream tests
  fail because the alive count never returned to zero.

- `weak_ptr` is not copyable into a raw pointer or back into a
  bare `shared_ptr` without `lock()`. If you find yourself
  writing `c->parent.lock()->name` chained without checking,
  remember: `lock()` can return empty. Bind the result and check
  it.

- This pattern (own one way, observe the other) is the standard
  C++ tree/graph idiom. It generalises to siblings, doubly-linked
  lists, observers -- anywhere two objects refer to each other.
