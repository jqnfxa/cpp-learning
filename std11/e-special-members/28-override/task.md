# 28 — `override`

> Spec: <https://en.cppreference.com/cpp/language/override>

## What you learn

Before C++11, the way to override a virtual function in a derived
class was to redeclare it with the same signature — and *hope* you
got the signature right. If your derived declaration was off by a
`const`, by an argument type, or by anything else, you didn't get
an override; you got a brand-new function that happened to share a
name. The base's virtual was unaffected, and calls through a base
pointer kept reaching the base.

```cpp
class base
{
public:
    virtual void process(const std::string &s) const;
};

class derived : public base
{
public:
    /* Caller's intent: override base::process. */
    /* Actual effect: brand-new method, NOT an override. */
    void process(std::string s) const;     /* by value, not const ref */
};

base *p = new derived;
std::string s = "hi";
p->process(s);     /* calls base::process, NOT derived::process */
```

C++11 introduced **`override`**: a contextual keyword you put after
a virtual member function declaration to assert "I am overriding a
base virtual". If the assertion is wrong (no matching base function),
the compiler refuses to build.

```cpp
class derived : public base
{
public:
    void process(std::string s) const override;
    /*                              ^^^^^^^^ */
    /*                              error: 'process' marked override
                                    but does not override */
};
```

Always use `override` on derived-class virtuals. It catches a whole
category of bugs at compile time.

## Task

In `solution.hpp`, `task28::base` is provided. Declare
`task28::derived : public base` with an override of `process` that
returns `2` (base returns `1`). The override **must** be marked with
the `override` keyword, with the *exact* matching signature:

```cpp
class base
{
public:
    virtual ~base() = default;
    virtual int process(const std::string &input) const;
};

class derived : public base
{
public:
    int process(const std::string &input) const override;  /* return 2 */
};
```

`tests.cpp` covers:
- Direct calls on `base` and `derived` instances.
- Virtual dispatch through a `base *` to a `derived` (returns 2).
- A test that confirms the signature matches by calling through a
  `base *` with a *temporary* `std::string` (this only works if
  the parameter type is exactly `const std::string &`).

## Run

```sh
./verify.sh
```

## Hints

- The override goes after the `const`:
  ```cpp
  int process(const std::string &input) const override;
  ```
  Order matters less than getting the four parts present: the
  return type, the parameter list, the `const` qualifier, and the
  `override` keyword.

- A common mistake — easy to make and easy to catch with `override`:
  ```cpp
  int process(std::string input) const override;     /* by-value param */
  /* error: marked 'override' but does not override anything */
  ```

- `override` is contextual — it's not a reserved keyword. You can
  still name a variable `override`. The compiler only interprets it
  as the override-keyword in member-function declarator context.
