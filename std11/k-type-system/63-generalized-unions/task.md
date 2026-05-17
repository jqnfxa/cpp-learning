# 63 — generalized (non-trivial) unions

> Spec: <https://en.cppreference.com/cpp/language/union>

## What you learn

C++11 lifted the C++03 restriction that a union member must be a
*trivial* type. After C++11, a union may contain *any* type — even
ones with non-trivial constructors, destructors, copy/move
operations (e.g., `std::string`, `std::vector`). The price is that
the compiler stops generating these special members for the union
itself: you have to write them by hand and orchestrate them via
*placement new* and explicit destructor calls.

The pattern that makes this safe is the **discriminated union**
(a.k.a. tagged union): the union sits next to a small enum (or
`int`) that records which alternative is currently alive, and
every operation reads the tag, then dispatches to the right
in-place construct / destroy / copy / move.

```cpp
struct int_or_string
{
    enum tag_t { is_int, is_string };

    int_or_string(int n) : tag(is_int), as_int(n) {}

    int_or_string(const std::string &s) : tag(is_string)
    {
        new (&as_string) std::string(s);  /* placement new */
    }

    ~int_or_string()
    {
        if (tag == is_string)
        {
            as_string.~basic_string();  /* explicit destructor */
        }
    }

    tag_t tag;
    union
    {
        int         as_int;
        std::string as_string;
    };
};
```

Two things to internalise:

1. **The union itself has no automatic lifetime for non-trivial
   members.** Writing `as_string = "hi"` on a freshly-default-
   constructed `int_or_string` is undefined behaviour: no string
   object lives there yet. You must `new (&as_string) std::string("hi")`
   to *start* its lifetime.

2. **You must end the lifetime explicitly.** When you switch the
   active member, call the destructor of the old one
   (`as_string.~basic_string()`) before constructing the new one.

The destructor of the union *itself* is deleted by the compiler
(because it cannot know which alternative is alive). The
containing class brings it back by defining its own destructor
that consults the tag.

This is the C++11 mechanism that `std::variant` (C++17) wraps up
into a type-safe library facility. Writing one by hand once is a
rite of passage.

## Task

In `solution.hpp`, complete the `int_or_string` class so it can
hold either an `int` or a `std::string`, with placement-new in
each branch and a discriminator tag.

The class must support:

- Construction from `int`.
- Construction from `std::string` (and from `const char *` via the
  string's implicit conversion).
- `which()` returning the active tag.
- `as_int()` and `as_string()` accessors (no checking — UB if the
  wrong one is asked for, mirroring real unions).
- `assign(int)` and `assign(const std::string &)` that correctly
  end the old member's lifetime before starting the new one's.
- A destructor that destroys the active member.
- Copy constructor: deep-copy whichever alternative is active.

```cpp
class int_or_string
{
public:
    enum tag_t
    {
        is_int,
        is_string
    };

    int_or_string(int n);
    int_or_string(const std::string &s);
    int_or_string(const int_or_string &other);
    ~int_or_string();

    tag_t which() const noexcept;
    int as_int() const noexcept;
    const std::string &as_string() const noexcept;

    void assign(int n);
    void assign(const std::string &s);

private:
    tag_t tag_;
    union
    {
        int int_;
        std::string string_;
    };
};
```

## Run

```sh
./verify.sh
```

## Hints

- Placement new lives in `<new>`. Syntax: `new (address) T(args...)`.

- For the explicit destructor call, the typedef-name of the type
  works: `string_.~basic_string()` or, more portably,
  `string_.std::string::~string()`. The first form is the one you
  see in the wild.

- Constructors with placement new must initialise `tag_` first (or
  alongside) so a *subsequent* exception during placement-new
  leaves the object in a state the destructor can handle. For this
  task we trust `std::string`'s small-string optimisation and
  don't worry about constructor failure mid-flight.

- The default constructor of the union *is* implicitly defined as
  deleted (because `std::string` is non-trivial), which is why the
  containing class never invokes it: every constructor reaches
  the union members via placement new.

- `assign` is the only non-trivial state transition: read tag, if
  changing type, call destructor on the old branch, then placement-
  new into the new branch, then update tag. If the type isn't
  changing, you can just assign normally — the existing alternative
  is already alive.

- C++17's `std::variant<int, std::string>` is the modern answer
  and removes the need for any of this hand-rolled machinery. For
  C++11, you write it yourself.
