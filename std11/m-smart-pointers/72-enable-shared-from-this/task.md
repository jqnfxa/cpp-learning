# 72 — `std::enable_shared_from_this`

> Spec: <https://en.cppreference.com/cpp/memory/enable_shared_from_this>

## What you learn

Suppose you have a class that needs to register itself as a
listener somewhere. The callback list owns `shared_ptr<T>`, so
the method has to give *itself* away as a `shared_ptr<T>`. The
naive attempt:

```cpp
struct Widget
{
    void subscribe(std::vector<std::shared_ptr<Widget>> &list)
    {
        list.push_back(std::shared_ptr<Widget>(this));  /* BUG */
    }
};

auto w = std::make_shared<Widget>();
w->subscribe(roster);
```

This is **undefined behaviour**. The original `make_shared` set
up a control block for `w`. The naive constructor inside
`subscribe` makes a *second* `shared_ptr` with a *second*
control block pointing at the same object. When either count
hits zero the object is destroyed; when the other count
eventually hits zero, the object is destroyed *again*. Double
free, often presenting as memory corruption that surfaces ten
function calls later.

`std::enable_shared_from_this<T>` is the library facility that
fixes this. You inherit publicly from it:

```cpp
class Widget : public std::enable_shared_from_this<Widget> { ... };
```

The base class stores a `weak_ptr<T>` to `this`. When the object
is first owned by a `shared_ptr` (via `make_shared` or via the
`shared_ptr(T *)` constructor), the `shared_ptr` machinery
detects the `enable_shared_from_this` base and wires up that
internal `weak_ptr` to the object's existing control block.

Now, inside a member function, you can call:

```cpp
std::shared_ptr<Widget> sp = shared_from_this();
```

`shared_from_this()` locks the internal `weak_ptr` and returns a
`shared_ptr` that shares ownership with the *original* control
block. No second control block, no double free.

### The "must already be shared" requirement

`shared_from_this()` only works after the object is already
owned by *some* `shared_ptr`. If you call it inside the
constructor (or before any `shared_ptr<T>` ever wrapped the
object), the internal weak_ptr is empty and you get
`std::bad_weak_ptr` thrown.

```cpp
struct Widget : std::enable_shared_from_this<Widget>
{
    Widget()
    {
        auto sp = shared_from_this();  /* throws bad_weak_ptr */
    }
};
```

Two consequences:

- Don't call `shared_from_this()` from a constructor.
- Don't create a `Widget` on the stack and expect
  `shared_from_this()` to work — the object was never owned by
  a `shared_ptr` in the first place.

### `weak_from_this()` (C++17)

C++17 added `weak_from_this()` which returns the internal
`weak_ptr` directly. It doesn't throw — an empty `weak_ptr` is
fine. For C++11 you only have `shared_from_this()`.

## Task

Two changes in `solution.hpp`:

1. Make `Widget` inherit publicly from
   `std::enable_shared_from_this<Widget>`.
2. Implement `Widget::self()` to return
   `shared_from_this()`, and `Widget::subscribe(roster)` to push
   `shared_from_this()` into the roster.

```cpp
class Widget : public std::enable_shared_from_this<Widget>  /* TODO */
{
    int value_;
public:
    explicit Widget(int v);
    int value() const;

    /* TODO: return shared_from_this(). */
    std::shared_ptr<Widget> self();

    /* TODO: roster.push_back(shared_from_this()). */
    void subscribe(std::vector<std::shared_ptr<Widget>> &roster);
};

inline std::shared_ptr<Widget> make_widget(int v)
{
    return std::make_shared<Widget>(v);
}
```

## Run

```sh
./verify.sh
```

## Hints

- `self()` is one line. `subscribe()` is also one line.

- Inherit *publicly* from `enable_shared_from_this<Widget>`. A
  private base would make `shared_from_this()` inaccessible
  outside `Widget`'s own member functions — usually not what you
  want, and the standard pattern is public.

- `enable_shared_from_this<T>` does not change `sizeof(T)` much
  (it adds one `weak_ptr`-sized member to the *base*, which is
  one pointer in most implementations). It's a fine
  always-on facility for any class that might hand out
  `shared_ptr`s to itself.

- The internal weak_ptr is wired up at the moment a `shared_ptr`
  takes ownership. The detection is via the
  `enable_shared_from_this` base. If you forget to inherit, the
  wiring doesn't happen and `shared_from_this()` would either
  throw or fail to compile depending on usage.

- `make_widget()` returning `make_shared<Widget>` is the
  canonical paired idiom: the factory hands out the first
  `shared_ptr`, which wires up the internal weak_ptr; subsequent
  `self()` / `subscribe()` calls succeed.

- The classic interview question that uses this: "How do you
  hand out `shared_ptr<this>` safely from a member function?".
  Answer: inherit from `enable_shared_from_this`, call
  `shared_from_this()`. The trap is that without it, calling
  `shared_ptr<T>(this)` creates a second control block.
