# 27 — Trigraphs and `register` removed

C++17 removed:

- **Trigraphs**: `??=`, `??<`, `??>`, `??(`, `??)`, etc.
  Replaced with normal punctuation in source.
- **`register`** as a storage-class specifier. The keyword
  is still reserved (can't be used as an identifier).

This is a read-only conceptual step. The lack of trigraphs
and the keyword `register` not being usable as a variable
name is the "change."

```cpp
register int x = 5;   /* C++17: error -- register is reserved */
```
