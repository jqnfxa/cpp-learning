# 6 — Temporary materialization

> Spec: <https://en.cppreference.com/cpp/language/copy_elision>

## What you learn

C++17 introduced the term **temporary materialization** to
describe the precise moment when a prvalue becomes a
temporary object in memory. The old rule "binding a prvalue
to a const-reference extends its lifetime" still applies --
but the standard now phrases it in terms of materialization:

```cpp
struct trace { /* ... */ };

const trace& t = trace(99);    /* temp materialised; lifetime
                                 extended to t's scope */
// t.id is accessible here
// t's destructor runs when this scope ends
```

The destructor runs at the **end of the scope containing the
reference**, not at the end of the full-expression where the
temporary was created. That's the lifetime extension.

### Reading the rule

Pre-C++17 phrasing: "a temporary bound to a const-ref has
its lifetime extended."

C++17 phrasing: "the prvalue is **materialized** into a
temporary object (via 'temporary materialization
conversion') at the point where a glvalue is needed for it
-- e.g., to bind a reference. The resulting temporary has
lifetime extension."

Same observable behavior; cleaner semantic model. The
prvalue itself has no storage; the *temporary* that it
materializes into does.

### The lifetime-extension rules

Lifetime extension applies to:

- `const T& x = prvalue_expression;` -- the prvalue is
  materialized into a temp, the temp lives as long as `x`.
- `T&& x = prvalue_expression;` -- same rule with rvalue
  ref.
- Member-of-temporary used in the same way: `auto& x =
  make_pair(1, 2).first;` -- the pair's lifetime extends,
  not just `.first`.

Lifetime extension does NOT apply to:

- A reference returned from a function -- the temp is
  destroyed when the calling expression ends, then the
  returned reference dangles:
  ```cpp
  const T& dangling = some_function_returning_const_T_ref();
  /* the function created a temp internally, but it's gone now */
  ```
- A reference bound to a member through a function:
  ```cpp
  const int& x = make_pair(1, 2.0).second;  /* OK, extension */
  const int& y = get_second(make_pair(1, 2.0));  /* NOT extended */
  ```

The first works because the prvalue (the pair) is on the
spot. The second has a function returning a reference --
the function's view of the pair is gone before `y` reads
it.

### What changes are observable in C++17

For most user code, NOTHING -- the lifetime-extension
behavior matches the C++14 implementation. What changed is
the *standard wording* and a few corner-case behaviors:

1. **`prvalue` is no longer a kind of object**: it's a
   pure expression result. Materialization is the moment
   the prvalue becomes an object.
2. **Guaranteed elision** (step 5) is layered on top:
   when initializing from a prvalue, no temporary is
   materialized at all.
3. **Class types** can be returned even when non-movable
   (step 5).

You'd write the same code, but the model is now precise.

### The lifetime-extension trap

A common pitfall: binding a function-returned-reference to
a const-ref:

```cpp
const T& dangling_temp() {
    return T();      /* error: returns ref to temp */
}

const T& x = dangling_temp();   /* dangling! */
```

The compiler usually warns about returning a reference to a
local/temporary, but if it slips through, you have a
dangling reference. No lifetime extension across function
returns.

### Returning by value AND binding to const-ref

```cpp
T factory() { return T(args); }   /* returns by value */

const T& r = factory();           /* OK: binds to a prvalue */
```

The `factory()` call is a prvalue (the materialized
temporary). The const-ref binds to it and extends its
lifetime to `r`'s scope. This is safe.

(In C++17, guaranteed elision means `T x = factory();`
constructs directly. But `const T& r = factory();` still
materializes the temp, since the LHS is a reference, not a
value.)

### Detecting lifetime extension in tests

The test pattern uses a destructor that bumps a counter.
Inside a scope, you bind a prvalue to a const-ref and check
the counter:

```cpp
trace::reset();
{
    const trace& t = trace(99);   /* lifetime extended */
    EXPECT_EQ(trace::destroyed_count(), 0);  /* still alive */
}
EXPECT_EQ(trace::destroyed_count(), 1);  /* destroyed at scope end */
```

If the destructor count is 1 INSIDE the scope, the extension
didn't happen; you have a dangling reference.

Stub form: doesn't bind to a const-ref at all -- just creates
the temp and discards it:

```cpp
(void)trace(99);   /* temp dies at the semicolon */
```

Inside the same imaginary scope, the destruction count is
already 1.

### Const-ref vs rvalue-ref binding

Both extend lifetime:

```cpp
const trace& a = trace(1);   /* extension via const-ref */
trace&& b = trace(2);        /* extension via rvalue-ref */
```

`a` is `const trace&`; `b` is `trace&&` (and is itself an
lvalue at this point -- references are lvalues). Both
extend their temporaries until the binding goes out of
scope.

### Multiple temporaries in one scope

```cpp
{
    const trace& a = trace(1);
    const trace& b = trace(2);
    const trace& c = trace(3);
    /* all three temps alive */
}
/* all three destroyed in reverse declaration order */
```

The destructors run in reverse order (c first, then b, then
a), per the usual scope-end rules.

## Task

In `solution.hpp`, fill in two functions:

```cpp
inline int destruction_count_within_scope() {
    trace::reset();
    int snapshot;
    {
        const trace& t = trace(99);
        (void)t;
        snapshot = trace::destroyed_count();
    }
    return snapshot;
}

inline int peek_through_ref() {
    trace::reset();
    const trace& t = trace(42);
    return t.id;
}
```

Both bind a prvalue `trace(...)` to a `const trace&` and
observe behavior that depends on the binding extending the
temporary's lifetime.

## Run

```sh
./verify.sh
```

## Hints

- `trace` has its copy/move all `delete`d. The const-ref
  binding doesn't move or copy -- it just references the
  materialised temp directly.

- `(void)t;` is the trick for "I want this name to be
  alive but not used" without a compile warning.

- For `destruction_count_within_scope`: the snapshot is
  taken WHILE the const-ref is in scope. With extension,
  the trace hasn't been destroyed yet -- count is 0. The
  stub `(void)trace(99)` creates a temp and lets it die at
  the semicolon; count is 1 before snapshot is taken.

- For `peek_through_ref`: simply read `t.id`. With the
  const-ref binding, `t` is alive when `t.id` is read. The
  stub returns 0 because no binding is established.

- `manual_const_ref_extension`: a test that demonstrates
  the pattern inline (without calling our helper). Used as
  the spec example.

- `multiple_temporaries_extended`: shows that multiple
  bindings in one scope all extend their respective temps.
  After the scope ends, all are destroyed (in reverse
  order).

- `rvalue_ref_also_extends`: `T&& r = T(99);` extends just
  like const-ref. The reference itself is still an lvalue
  inside the scope.

- The `count_increments_on_each_destruction` test is a
  sanity check on the trace counter.

- C++17 didn't change the OBSERVABLE behavior here -- it
  changed the standard wording (temporary materialization).
  For your purposes: const-ref + prvalue + scope = safe
  lifetime extension.

- The trap: returning a reference from a function does NOT
  extend lifetime across the function boundary. Bind the
  prvalue at the call site, not behind a wrapper that
  returns a reference.
