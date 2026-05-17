# 38 — `std::invoke`

Uniform call syntax for any callable: free functions,
function pointers, member function pointers, member object
pointers, lambdas, function objects.

```cpp
std::invoke(free_func, x);          /* free_func(x) */
std::invoke(&Class::method, obj);   /* obj.method() */
std::invoke(&Class::field, obj);    /* obj.field */
std::invoke([](int x){return x;}, 1);
```

Used by `std::apply`, `std::function`, etc.
