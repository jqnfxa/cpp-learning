# C++17 — step-by-step plan

C++17 brings vocabulary types (`optional`, `variant`, `any`,
`string_view`), parallelism, filesystem, structured bindings, and a much
richer `constexpr`. Prerequisite: std11/ and std14/.

Per-task layout (same as earlier):

    std17/<section>/<NN-name>/
        task.md       — exercise spec
        solution.hpp  — you write this
        tests.cpp     — pre-written tests (GoogleTest)
        verify.sh     — build + run tests
        CMakeLists.txt

---

## Part I — Language

### A. Initialization and bindings

1. **Structured bindings: arrays** — [spec](https://en.cppreference.com/cpp/language/structured_binding). *Task: `int arr[3] = {1,2,3}; auto [a,b,c] = arr;`.*
2. **Structured bindings: tuples and pairs** — same spec. *Task: split the result of `std::map::insert` into `[it, inserted]`.*
3. **Structured bindings: structs (public data members)** — same spec. *Task: `struct point{int x,y;}; auto [x,y] = make_point();`.*
4. **`if` and `switch` with init-statement** — [spec](https://en.cppreference.com/cpp/language/if). *Task: rewrite `auto it = map.find(k); if (it != map.end()) {...}` as `if (auto it = map.find(k); it != map.end()) {...}`.*
5. **Guaranteed copy elision** — [spec](https://en.cppreference.com/cpp/language/copy_elision). *Task: return a non-movable type from a factory function and show it works.*
6. **Temporary materialization** — same spec. *Task: bind a prvalue to a `const &` and observe lifetime extension.*

### B. Templates and deduction

7. **Class template argument deduction (CTAD)** — [spec](https://en.cppreference.com/cpp/language/class_template_argument_deduction). *Task: write `std::pair p{1, 2.5};` and `std::vector v{1, 2, 3};` with no explicit template args.*
8. **User-defined deduction guides** — same spec. *Task: write a deduction guide for a custom `wrapper` template.*
9. **Fold expressions** — [spec](https://en.cppreference.com/cpp/language/fold). *Task: implement `sum(args...)` as `(... + args)`.*
10. **Auto non-type template parameters** — [spec](https://en.cppreference.com/cpp/language/template_parameters). *Task: write `template<auto N> struct fixed_array { int data[N]; };` and instantiate with `<5>` and `<10ULL>`.*

### C. `constexpr` continues

11. **`if constexpr`** — [spec](https://en.cppreference.com/cpp/language/if). *Task: write a generic `print(v)` that dispatches to `<<` for streamable types and to `to_string` otherwise via `if constexpr`.*
12. **`constexpr` lambdas** — [spec](https://en.cppreference.com/cpp/language/lambda). *Task: write a `constexpr` lambda used in a `static_assert`.*

### D. Lambdas continued

13. **Lambda capture of `*this`** — [spec](https://en.cppreference.com/cpp/language/lambda). *Task: in a member function, capture `*this` (by value) and pass the lambda to a thread; mutate without dangling.*

### E. Inline things

14. **`inline` variables** — [spec](https://en.cppreference.com/cpp/language/inline). *Task: define a header-only `inline int global_counter = 0;` and verify a single definition across two TUs.*
15. **Nested namespaces (`namespace a::b::c`)** — [spec](https://en.cppreference.com/cpp/language/namespace). *Task: replace 3 levels of nested `namespace { ... }` with a single declaration.*

### F. Order of evaluation

16. **New order-of-evaluation rules** — [spec](https://en.cppreference.com/cpp/language/eval_order). *Task: read-only; explain in a comment which of `f(g(), h())` orderings are now defined.*

### G. Type system and attributes

17. **`noexcept` is part of the type** — [spec](https://en.cppreference.com/cpp/language/noexcept_spec). *Task: show that `int(*)(int) noexcept` is not convertible to `int(*)(int)` and the reverse is.*
18. **`u8` character literal** — [spec](https://en.cppreference.com/cpp/language/character_literal). *Task: write `constexpr char c = u8'A';` and assert its value.*
19. **`[[fallthrough]]`** — [spec](https://en.cppreference.com/cpp/language/attributes/fallthrough). *Task: in a switch with deliberate fall-through, suppress the warning with `[[fallthrough]]`.*
20. **`[[nodiscard]]`** — [spec](https://en.cppreference.com/cpp/language/attributes/nodiscard). *Task: mark a `must_read()` return as `[[nodiscard]]`; show the warning when discarded.*
21. **`[[maybe_unused]]`** — [spec](https://en.cppreference.com/cpp/language/attributes/maybe_unused). *Task: silence an unused-parameter warning on a stub function.*
22. **`__has_include`** — [spec](https://en.cppreference.com/cpp/preprocessor/include). *Task: include `<optional>` if available, else fall back to a hand-rolled stub.*

### H. Removals (be aware of them)

23. **Removed: `std::auto_ptr`** — [spec](https://en.cppreference.com/cpp/memory/auto_ptr). *Task: rewrite a code snippet that uses `auto_ptr` to use `unique_ptr`.*
24. **Removed: `std::random_shuffle`** — [spec](https://en.cppreference.com/cpp/algorithm/random_shuffle). *Task: replace it with `std::shuffle` + an engine.*
25. **Removed: `std::unexpected` and dynamic exception specs** — [spec](https://en.cppreference.com/cpp/language/except_spec). *Task: replace `throw(X, Y)` with `noexcept(false)` / no spec.*
26. **Deprecated: `std::iterator`** — [spec](https://en.cppreference.com/cpp/iterator/iterator). *Task: rewrite a hand-rolled iterator by exposing its 5 typedefs directly.*
27. **Trigraphs and `register` are gone** — [spec](https://en.cppreference.com/cpp/language/operator_alternative). *Task: read-only; verify `register int x;` no longer compiles.*

---

## Part II — Library

### I. Vocabulary types

28. **`std::optional<T>`** — [spec](https://en.cppreference.com/cpp/utility/optional). *Task: write `parse_int(s)` returning `optional<int>`.*
29. **`optional` operations: `value_or`, `has_value`, `*` and `->`** — same spec. *Task: chain `parse_int` with a default via `value_or(0)`.*
30. **`std::variant<Ts...>`** — [spec](https://en.cppreference.com/cpp/utility/variant). *Task: model a `JSON` value as `variant<nullptr_t, bool, double, string, vector<...>, map<...>>`.*
31. **`std::visit`** — same spec. *Task: print a `variant<int, std::string>` via `visit` and an overload-set helper.*
32. **`std::any`** — [spec](https://en.cppreference.com/cpp/utility/any). *Task: store and retrieve heterogeneous values; show `any_cast` failure throws.*
33. **`std::string_view`** — [spec](https://en.cppreference.com/cpp/string/basic_string_view). *Task: rewrite a `count_words(std::string const &)` to take `std::string_view`.*

### J. Tuple plumbing

34. **`std::apply`** — [spec](https://en.cppreference.com/cpp/utility/apply). *Task: unpack a `tuple<int, int, int>` into a function call with `std::apply`.*
35. **`std::make_from_tuple`** — [spec](https://en.cppreference.com/cpp/utility/make_from_tuple). *Task: construct a `point{x, y}` from a `tuple<int, int>`.*

### K. Function utilities

36. **`std::as_const`** — [spec](https://en.cppreference.com/cpp/utility/as_const). *Task: pass a non-const container to a function that wants `const &` without making a copy.*
37. **`std::not_fn`** — [spec](https://en.cppreference.com/cpp/utility/functional/not_fn). *Task: rewrite `std::count_if(.., [](auto x){return !pred(x);})` using `not_fn(pred)`.*
38. **`std::invoke`** — [spec](https://en.cppreference.com/cpp/utility/functional/invoke). *Task: call a member function pointer, a free function, and a functor uniformly.*

### L. Searchers

39. **`std::search` with `default_searcher` / `boyer_moore_searcher` / `boyer_moore_horspool_searcher`** — [spec](https://en.cppreference.com/cpp/algorithm/search). *Task: pattern-match a long needle in a long haystack three different ways and compare timings.*

### M. Memory and allocation

40. **`std::byte`** — [spec](https://en.cppreference.com/cpp/types/byte). *Task: write a raw-byte buffer printer using `std::byte`.*
41. **`std::launder`** — [spec](https://en.cppreference.com/cpp/utility/launder). *Task: read-only; in a comment, describe a case where it's required.*
42. **`std::aligned_alloc`** — [spec](https://en.cppreference.com/cpp/memory/c/aligned_alloc). *Task: allocate a 64-byte-aligned buffer and free it.*
43. **Aligned `operator new`** — [spec](https://en.cppreference.com/cpp/memory/new/operator_new). *Task: define `alignas(64) struct cache_line {...};` and confirm the overload is selected.*
44. **`std::pmr::memory_resource` and `std::pmr::polymorphic_allocator`** — [spec](https://en.cppreference.com/cpp/memory/memory_resource). *Task: a `std::pmr::vector<int>` backed by a `std::pmr::monotonic_buffer_resource` on the stack.*
45. **Uninitialized algorithms: `destroy_at`, `destroy`, `destroy_n`, `uninitialized_move`, `uninitialized_value_construct`** — [spec](https://en.cppreference.com/cpp/memory/destroy_at). *Task: implement a tiny growable vector using these (no `delete[]`).*
46. **`weak_from_this`** — [spec](https://en.cppreference.com/cpp/memory/enable_shared_from_this/weak_from_this). *Task: extend the std11 step 72 class to hand out a `weak_ptr` to itself.*
47. **Array support for `std::shared_ptr`** — [spec](https://en.cppreference.com/cpp/memory/shared_ptr). *Task: own a `shared_ptr<int[]>` and access elements.*

### N. Type traits and metaprogramming

48. **Variable templates of type traits (`is_integral_v`, `is_same_v`, ...)** — [spec](https://en.cppreference.com/cpp/header/type_traits). *Task: rewrite std11 step 102's traits using the `_v` form.*
49. **`std::conjunction`, `std::disjunction`, `std::negation`** — [spec](https://en.cppreference.com/cpp/types/conjunction). *Task: write a short-circuiting `enable_if_t<conjunction_v<is_integral<T>, is_signed<T>>>` constraint.*
50. **`std::is_invocable` / `is_invocable_r`** — [spec](https://en.cppreference.com/cpp/types/is_invocable). *Task: assert that `is_invocable_v<F, int>` is true for a callable taking an int.*
51. **`std::is_swappable` / `is_nothrow_swappable`** — [spec](https://en.cppreference.com/cpp/types/is_swappable). *Task: gate a generic `relocate(a, b)` on `is_nothrow_swappable_v<T>`.*
52. **`std::is_aggregate`** — [spec](https://en.cppreference.com/cpp/types/is_aggregate). *Task: assert aggregate-ness for a plain struct and non-aggregate-ness for a class with a user-defined ctor.*
53. **`std::has_unique_object_representations`** — [spec](https://en.cppreference.com/cpp/types/has_unique_object_representations). *Task: prove `int` has it and a float-containing struct does not.*

### O. Algorithms and numerics

54. **`std::clamp`** — [spec](https://en.cppreference.com/cpp/algorithm/clamp). *Task: clamp a vector of ints into `[0, 100]`.*
55. **Parallel algorithms with `<execution>`** — [spec](https://en.cppreference.com/cpp/algorithm/execution_policy_tag_t). *Task: sort a vector with `std::execution::par` and compare wall time to sequential.*
56. **`std::reduce`** — [spec](https://en.cppreference.com/cpp/algorithm/reduce). *Task: sum a vector with `reduce(par, ...)`; note when the result can differ from `accumulate` (associativity).*
57. **`std::inclusive_scan` / `std::exclusive_scan`** — [spec](https://en.cppreference.com/cpp/algorithm/inclusive_scan). *Task: compute prefix sums two ways and explain the off-by-one in a comment.*
58. **`std::gcd` and `std::lcm`** — [spec](https://en.cppreference.com/cpp/numeric/gcd). *Task: reduce a fraction `(a/b)` by `gcd(a, b)`.*
59. **`std::hypot(x, y, z)` (3D overload)** — [spec](https://en.cppreference.com/cpp/numeric/math/hypot). *Task: compute the magnitude of a 3D vector.*
60. **Mathematical special functions (`std::beta`, `std::cyl_bessel_j`, ...)** — [spec](https://en.cppreference.com/cpp/numeric/special_functions). *Task: pick one (Bessel J₀) and verify a tabulated value.*

### P. Container additions

61. **`map::try_emplace` and `insert_or_assign`** — [spec](https://en.cppreference.com/cpp/container/map/try_emplace). *Task: word-count without double-construction of the value type.*
62. **`map`/`set` `extract` and `merge`** — [spec](https://en.cppreference.com/cpp/container/map/extract). *Task: move a node from `map_a` to `map_b` without reallocation.*

### Q. Iterators and contiguity

63. **`LegacyContiguousIterator` concept (named requirement)** — [spec](https://en.cppreference.com/cpp/named_req/ContiguousIterator). *Task: write a function that only compiles when given a contiguous iterator (via SFINAE on `std::is_pointer_v<T*>`-style trick or `iterator_traits`).*
64. **Non-member `std::size`, `std::empty`, `std::data`** — [spec](https://en.cppreference.com/cpp/iterator/size). *Task: write a generic `print_first(c)` using `std::data` and `std::size`.*

### R. I/O and strings

65. **`std::to_chars` and `std::from_chars`** — [spec](https://en.cppreference.com/cpp/utility/to_chars). *Task: format and parse an `int` without allocation; compare to `std::to_string`.*

### S. Exception handling

66. **`std::uncaught_exceptions` (plural, returns count)** — [spec](https://en.cppreference.com/cpp/error/uncaught_exception). *Task: implement an RAII scope-guard that runs different code on success vs unwinding.*

### T. Atomics

67. **`std::atomic<T>::is_always_lock_free`** — [spec](https://en.cppreference.com/cpp/atomic/atomic/is_always_lock_free). *Task: assert at compile time that `atomic<int>` is always lock-free on your platform.*

### U. Concurrency

68. **`std::scoped_lock`** — [spec](https://en.cppreference.com/cpp/thread/scoped_lock). *Task: lock two mutexes at once without deadlock using `scoped_lock`.*

### V. Filesystem

69. **`std::filesystem::path`** — [spec](https://en.cppreference.com/cpp/filesystem/path). *Task: join a directory path with a filename portably.*
70. **`directory_iterator` and `recursive_directory_iterator`** — [spec](https://en.cppreference.com/cpp/filesystem/directory_iterator). *Task: print all regular files under a given directory.*
71. **`file_size`, `last_write_time`, `is_regular_file`** — [spec](https://en.cppreference.com/cpp/filesystem). *Task: report total bytes of `*.cpp` under a tree.*
72. **`copy`, `create_directories`, `remove_all`** — same spec. *Task: copy a small tree from `A/` to `B/`.*

### W. Polymorphic allocator helpers

73. **`std::pmr::polymorphic_allocator` as a vector allocator** — [spec](https://en.cppreference.com/cpp/memory/polymorphic_allocator). *Task: `std::pmr::vector<int>` over a `synchronized_pool_resource`.*

### X. Time

74. **`std::chrono::duration::round`, `floor`, `ceil`** — [spec](https://en.cppreference.com/cpp/chrono/duration/round). *Task: round a `std::chrono::microseconds` to nearest milliseconds.*

### Y. Misc

75. **`std::timespec_get`** — [spec](https://en.cppreference.com/cpp/chrono/c/timespec_get). *Task: get the wall time as a C `timespec`; convert to `system_clock::time_point`.*
76. **`std::hardware_destructive_interference_size` and `std::hardware_constructive_interference_size`** — [spec](https://en.cppreference.com/cpp/thread/hardware_destructive_interference_size). *Task: pad two atomics so they're on separate cache lines.*
