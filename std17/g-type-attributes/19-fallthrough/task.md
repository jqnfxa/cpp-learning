# 19 — `[[fallthrough]]`

> Spec: <https://en.cppreference.com/cpp/language/attributes/fallthrough>

## What you learn

C++17's `[[fallthrough]]` attribute marks an intentional
case-to-case fall-through in a switch, suppressing
compiler warnings about implicit fall-through.

```cpp
switch (x) {
    case 0:
        do_a();
        [[fallthrough]];   /* intentional -- continue into case 1 */
    case 1:
        do_b();
        break;
}
```

Without the attribute, GCC/Clang at `-Wimplicit-fallthrough`
warn that you might have forgotten a `break`.

## Task

Implement `classify(int)` with the case-0-falls-into-case-1
pattern. See the stub's TODO for the spec.

## Run

```sh
./verify.sh
```

## Hints

- `[[fallthrough]];` is a STATEMENT (note the `;`).

- It goes BEFORE the next `case` label, after all
  statements in the current case.

- For zero: result += "zero "; then [[fallthrough]]; falls
  into case 1 / case 2 which append "small". Total:
  "zero small".

- For 1 or 2: just "small".

- Without the attribute, this code triggers a warning at
  high warning levels. With it, the compiler knows it's
  intentional.
