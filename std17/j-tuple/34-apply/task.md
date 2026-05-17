# 34 — `std::apply`

> Spec: <https://en.cppreference.com/cpp/utility/apply>

Unpack a tuple into a function's positional arguments.
Replaces the C++14 `index_sequence` helper (std14 step 14):

```cpp
std::tuple<int, int> p{2, 3};
int s = std::apply([](int a, int b) { return a + b; }, p);   /* 5 */
int s2 = std::apply(my_function, p);   /* my_function(2, 3) */
```

## Hints

- Tuple's arity must match the callable's arity.
- Works with any callable: free function, lambda, member
  function pointer (via std::invoke under the hood).
