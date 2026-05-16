# 23 — Inheriting constructors with `using Base::Base;`

> Spec: <https://en.cppreference.com/cpp/language/using_declaration#Inheriting_constructors>

## What you learn

In C++03, a derived class did not inherit its base's constructors.
If `base` had five constructors and `derived` needed to be
constructible the same five ways, you had to write five forwarding
constructors:

```cpp
struct derived : base
{
    derived() : base() {}
    derived(int v) : base(v) {}
    derived(int v, std::string n) : base(v, std::move(n)) {}
    /* ... and so on */
};
```

That's a lot of boilerplate just to say "use whatever the base
offers". Worse, when someone adds a sixth constructor to `base`,
nothing reminds you to add the forwarding ctor in `derived`.

C++11 introduced **inheriting constructors**: one line brings *all*
of the base's constructors into the derived class.

```cpp
struct derived : base
{
    using base::base;
};
```

Now `derived d{42}` finds `base(int)`. `derived d{42, "x"}` finds
`base(int, std::string)`. And if you add `base(float)` later, it
shows up in `derived` for free.

Things to know:

1. **The body of an inherited constructor is `{}`.** The compiler
   synthesizes it. If your derived class has data members of its
   own, you need NSDMIs (step 21) or you have to write the
   constructors yourself.
2. **The base subobject is initialized normally.** The inherited
   constructor delegates to the base's constructor with matching
   arguments.
3. **The default constructor is implicitly declared if the base has
   one.** A `using base::base;` *does* include the default
   constructor.

## Task

`solution.hpp` provides `task23::base` with three constructors. You
declare `task23::derived` that publicly inherits from `base` and
inherits all three constructors via a single `using` declaration.

```cpp
class base
{
public:
    base();                                      /* value=0, name="default" */
    base(int v);                                 /* value=v, name="default" */
    base(int v, const std::string &n);           /* value=v, name=n         */

    int value() const;
    const std::string &name() const;
private:
    int value_;
    std::string name_;
};

class derived : public base
{
public:
    /* TODO: inherit every base ctor with one line */
};
```

| Construction              | `value()` | `name()`   |
|---------------------------|-----------|------------|
| `derived d{}`             | `0`       | `"default"`|
| `derived d{42}`           | `42`      | `"default"`|
| `derived d{42, "x"}`      | `42`      | `"x"`      |

## Run

```sh
./verify.sh
```

## Hints

- One line:
  ```cpp
  using base::base;
  ```
  Put it inside the `derived` class body, in the `public:` section.

- Do not write three forwarding constructors. That defeats the
  purpose of the step.

- The `value()` and `name()` accessors are members of `base` and
  are inherited normally (separate from the constructor question).
  They work on `derived` for free.

- `tests.cpp` also includes a `std::is_base_of` `static_assert` to
  pin the inheritance relationship.
