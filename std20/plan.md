# C++20 — step-by-step plan

C++20 is the biggest language jump since C++11: concepts, ranges,
coroutines, modules, `<=>`, formatted output, calendar/timezones, and
many quality-of-life library additions. Prerequisite: std11/, std14/,
std17/.

Per-task layout (same as earlier):

    std20/<section>/<NN-name>/
        task.md       — exercise spec
        solution.hpp  — you write this
        tests.cpp     — pre-written tests (GoogleTest)
        verify.sh     — build + run tests
        CMakeLists.txt

---

## Part I — Language

### A. Concepts and constraints

1. **`concept` definitions** — [spec](https://en.cppreference.com/cpp/language/constraints#Concepts). *Task: define `concept integral = std::is_integral_v<T>;` and `concept numeric = integral<T> || std::is_floating_point_v<T>;`.*
2. **`requires` clauses** — [spec](https://en.cppreference.com/cpp/language/constraints#Requires_clauses). *Task: constrain `template<integral T> T square(T);`.*
3. **`requires` expressions** — same spec. *Task: write `concept addable = requires(T a, T b) { a + b; };`.*
4. **Compound `requires` expressions (type, expression, nested)** — same spec. *Task: `concept iter_t = requires(I i) { typename I::value_type; { *i }; requires std::default_initializable<I>; };`.*
5. **Constrained `auto` (`std::integral auto x = ...`)** — [spec](https://en.cppreference.com/cpp/language/auto). *Task: declare `std::integral auto count = parse_int(s);` and demonstrate the failure mode.*
6. **Abbreviated function templates (`auto` in parameter lists)** — [spec](https://en.cppreference.com/cpp/language/function_template). *Task: write `void log(std::integral auto x);`.*
7. **Library concepts: `same_as`, `derived_from`, `convertible_to`** — [spec](https://en.cppreference.com/cpp/concepts). *Task: constrain a function to require `convertible_to<int>`.*
8. **Library concepts: `equality_comparable`, `totally_ordered`** — same spec. *Task: constrain `min(a, b)` to `totally_ordered`.*
9. **Library concepts: `regular`, `semiregular`, `movable`, `copyable`** — same spec. *Task: assert via `static_assert(std::regular<int>)` and write a non-regular type.*
10. **Library concepts: `invocable`, `predicate`, `relation`** — same spec. *Task: constrain a `count_if(c, pred)` on `predicate<value_type>`.*

### B. Three-way comparison

11. **`operator<=>` defaulted** — [spec](https://en.cppreference.com/cpp/language/default_comparisons). *Task: a `point{x,y}` with `auto operator<=>(const point &) const = default;` — get all six relational ops free.*
12. **`std::strong_ordering`, `weak_ordering`, `partial_ordering`** — [spec](https://en.cppreference.com/cpp/utility/compare). *Task: write a `case_insensitive_string` returning `weak_ordering` from `<=>`.*
13. **`operator==() = default`** — [spec](https://en.cppreference.com/cpp/language/default_comparisons). *Task: default `==` separately from `<=>`; show the conversion rules.*

### C. Other language features

14. **Designated initializers** — [spec](https://en.cppreference.com/cpp/language/aggregate_initialization#Designated_initializers). *Task: `struct opts { int retries = 3; bool verbose = false; }; opts{.retries = 5}`.*
15. **Init-statement in range-for** — [spec](https://en.cppreference.com/cpp/language/range-for). *Task: rewrite `auto v = make_vec(); for (auto x : v)` as `for (auto v = make_vec(); auto x : v)`.*
16. **`char8_t` and UTF-8 character literal** — [spec](https://en.cppreference.com/cpp/language/types#char8_t). *Task: write `const char8_t *s = u8"héllo";` and `static_assert(sizeof(s[0]) == 1);`.*
17. **`[[no_unique_address]]`** — [spec](https://en.cppreference.com/cpp/language/attributes/no_unique_address). *Task: empty-base optimization via `[[no_unique_address]] empty_t e;` instead of inheritance.*
18. **`[[likely]]` and `[[unlikely]]`** — [spec](https://en.cppreference.com/cpp/language/attributes/likely). *Task: annotate a hot/cold branch in a sort partition.*
19. **Pack expansion in lambda init-capture (`[...args = std::forward<Args>(args)]`)** — [spec](https://en.cppreference.com/cpp/language/lambda). *Task: capture a parameter pack into a lambda by perfect-forward.*
20. **`typename` no longer required in many contexts** — [spec](https://en.cppreference.com/cpp/language/dependent_name#The_typename_disambiguator_for_dependent_names). *Task: remove a redundant `typename` from a templated function and confirm it still compiles.*
21. **`consteval`** — [spec](https://en.cppreference.com/cpp/language/consteval). *Task: write a `consteval` `string_hash(const char *)` and use it in a `static_assert`.*
22. **`constinit`** — [spec](https://en.cppreference.com/cpp/language/constinit). *Task: define a `constinit` global table; show it's initialized at compile time but not implicitly `const`.*
23. **Further relaxed `constexpr` (try/catch, `new`/`delete`, virtual calls, unions)** — [spec](https://en.cppreference.com/cpp/language/constexpr). *Task: write a `constexpr` `std::vector`-like growable buffer used in a constant-evaluated context.*
24. **Signed integers are 2's complement** — [spec](https://en.cppreference.com/cpp/language/types#Properties). *Task: read-only; in a comment, list one C++17 UB that became defined in C++20.*
25. **Bitwise shifts: defined behavior** — [spec](https://en.cppreference.com/cpp/language/operator_arithmetic#Bitwise_shift_operators). *Task: read-only; in a comment, note the new well-defined rules for negative LHS shifts.*
26. **Aggregate initialization with parentheses** — [spec](https://en.cppreference.com/cpp/language/aggregate_initialization). *Task: `point p(1, 2);` for an aggregate `point` works in C++20.*
27. **Array `new[]` deduces size (DR11)** — [spec](https://en.cppreference.com/cpp/language/new#Array_form). *Task: `auto *p = new int[]{1, 2, 3};`.*

### D. Coroutines

28. **`co_await` / `co_yield` / `co_return`** — [spec](https://en.cppreference.com/cpp/language/coroutines). *Task: read-only; explain the three keywords in a comment.*
29. **A minimal `generator<T>` coroutine** — same spec. *Task: implement a hand-rolled `generator<int>` (promise_type, suspend_always) yielding `0..N`.*
30. **A minimal `task<T>` coroutine for async** — same spec. *Task: implement a `task<int>` that returns 42 after a `co_await suspend_never{}`.*

### E. Modules

31. **`export module`** — [spec](https://en.cppreference.com/cpp/language/modules). *Task: export a module `math` with one function; import it from `main.cpp`; build with `-fmodules-ts` or `-std=c++20 -fmodules` (compiler-dependent).*
32. **Module partitions** — same spec. *Task: split `math` into `math:basic` and `math:advanced` partitions.*

### F. Feature test

33. **Feature test macros (`__cpp_concepts`, `__cpp_lib_ranges`, ...)** — [spec](https://en.cppreference.com/cpp/feature_test). *Task: write a header that uses ranges if `__cpp_lib_ranges >= 201911L`, else falls back.*

---

## Part II — Library

### G. Ranges

34. **`std::ranges::sort`, `transform`, `find`** — [spec](https://en.cppreference.com/cpp/algorithm/ranges). *Task: sort a vector via `std::ranges::sort(v);` (no begin/end).*
35. **Range views: `views::filter`, `views::transform`** — [spec](https://en.cppreference.com/cpp/ranges). *Task: chain `v | views::filter(...) | views::transform(...)` and pull results into a new vector.*
36. **Range views: `views::take`, `views::drop`** — same spec. *Task: take the first 10 odd squares from an infinite source (`views::iota(1)`).*
37. **Range views: `views::iota`** — [spec](https://en.cppreference.com/cpp/ranges/iota_view). *Task: emit `views::iota(0, 10)` into a vector.*
38. **Range views: `views::reverse`, `views::elements`, `views::keys`, `views::values`** — same spec. *Task: from a `vector<pair<int,string>>`, extract `views::keys` as a separate vector.*
39. **Range views: `views::join`, `views::split`** — same spec. *Task: split `"a,b,,c"` by `,`; join `vector<vector<int>>` into a flat range.*
40. **`std::ranges::common_view`** — same spec. *Task: convert a non-common-range view into a common range so legacy algorithms accept it.*
41. **`std::ranges::subrange`** — [spec](https://en.cppreference.com/cpp/ranges/subrange). *Task: wrap an iterator pair into a range and pass to a `ranges::` algorithm.*

### H. Concepts library (already touched in Part I.A)

42. **Iterator concepts: `input_iterator`, `forward_iterator`, `random_access_iterator`, `contiguous_iterator`** — [spec](https://en.cppreference.com/cpp/iterator). *Task: write a function constrained to `random_access_iterator`.*

### I. Formatting

43. **`std::format`** — [spec](https://en.cppreference.com/cpp/utility/format). *Task: `std::format("name={}, score={:.2f}", "Anna", 9.5)`.*
44. **Custom `std::formatter<T>`** — same spec. *Task: specialize `formatter<my_type>` so `std::format("{}", my_type{...})` works.*

### J. Calendar and time zones

45. **`std::chrono::year_month_day`** — [spec](https://en.cppreference.com/cpp/chrono/year_month_day). *Task: build today's date from `floor<days>(system_clock::now())` and print it.*
46. **`std::chrono::hh_mm_ss`** — [spec](https://en.cppreference.com/cpp/chrono/hh_mm_ss). *Task: split a `duration` into hours/minutes/seconds.*
47. **`std::chrono::zoned_time` and `time_zone`** — [spec](https://en.cppreference.com/cpp/chrono/zoned_time). *Task: convert "now" between UTC and your local timezone.*

### K. Coroutine library

48. **`<coroutine>` infrastructure (`coroutine_handle`, `suspend_always`, `suspend_never`)** — [spec](https://en.cppreference.com/cpp/coroutine). *Task: in your step-29 generator, switch the initial-suspend from `suspend_always` to `suspend_never` and observe behavior.*

### L. Atomics and synchronization

49. **`std::atomic_ref<T>`** — [spec](https://en.cppreference.com/cpp/atomic/atomic_ref). *Task: increment a non-atomic field shared between threads using `atomic_ref`.*
50. **Atomic floating-point ops** — [spec](https://en.cppreference.com/cpp/atomic/atomic). *Task: `atomic<double>::fetch_add(1.0)`.*
51. **Atomic `std::shared_ptr`** — [spec](https://en.cppreference.com/cpp/memory/shared_ptr/atomic2). *Task: replace a manually-locked `shared_ptr<T>` slot with `atomic<shared_ptr<T>>`.*
52. **`std::counting_semaphore`** — [spec](https://en.cppreference.com/cpp/thread/counting_semaphore). *Task: a 3-permit semaphore gating a pool of workers.*
53. **`std::latch`** — [spec](https://en.cppreference.com/cpp/thread/latch). *Task: barrier-start N workers using a `latch(N)` so they all begin together.*
54. **`std::barrier`** — [spec](https://en.cppreference.com/cpp/thread/barrier). *Task: reusable barrier across multiple phases of a parallel computation.*
55. **`std::jthread` and cooperative cancellation (`stop_token`)** — [spec](https://en.cppreference.com/cpp/thread/jthread). *Task: a `jthread` worker that polls `get_stop_token().stop_requested()`.*
56. **`std::stop_source` / `stop_callback`** — [spec](https://en.cppreference.com/cpp/thread/stop_source). *Task: register a callback that fires on cancellation.*
57. **`std::basic_osyncstream`** — [spec](https://en.cppreference.com/cpp/io/basic_osyncstream). *Task: interleave logs from 4 threads without garbling lines.*

### M. Span and bit

58. **`std::span<T>` and `std::span<T, N>`** — [spec](https://en.cppreference.com/cpp/container/span). *Task: a `sum(span<const int>)` that accepts `vector`, `array`, C array.*
59. **`std::bit_cast<U>(t)`** — [spec](https://en.cppreference.com/cpp/numeric/bit_cast). *Task: reinterpret a `float` as `uint32_t` without UB.*
60. **`std::endian`** — [spec](https://en.cppreference.com/cpp/types/endian). *Task: `static_assert(std::endian::native == std::endian::little);` (or whatever your platform is).*
61. **`<bit>` power-of-two helpers: `bit_ceil`, `bit_floor`, `bit_width`, `has_single_bit`** — [spec](https://en.cppreference.com/cpp/numeric/bit_ceil). *Task: round 100 up to the next power of two with `bit_ceil`.*
62. **`<bit>` rotations: `rotl`, `rotr`** — [spec](https://en.cppreference.com/cpp/numeric/rotl). *Task: rotate a `uint32_t` left by 7.*
63. **`<bit>` popcount and counts: `popcount`, `countl_zero`, `countr_zero`** — [spec](https://en.cppreference.com/cpp/numeric/popcount). *Task: count set bits and leading zeros in `0x00ff00ff`.*

### N. New library types

64. **`std::source_location`** — [spec](https://en.cppreference.com/cpp/utility/source_location). *Task: implement a `log(msg, loc = source_location::current())` that prints file:line.*

### O. Smart pointers and memory

65. **`std::make_shared<T[]>`** — [spec](https://en.cppreference.com/cpp/memory/shared_ptr/make_shared). *Task: own a `shared_ptr<int[]>` via `make_shared<int[]>(10)`.*
66. **`std::make_unique_for_overwrite` / `make_shared_for_overwrite`** — [spec](https://en.cppreference.com/cpp/memory/unique_ptr/make_unique). *Task: allocate an uninitialized buffer (avoids the default-init cost) and immediately fill it.*
67. **`std::to_address`** — [spec](https://en.cppreference.com/cpp/memory/to_address). *Task: get the raw pointer out of a fancy pointer (or a plain `unique_ptr`) without dereferencing.*
68. **`std::assume_aligned`** — [spec](https://en.cppreference.com/cpp/memory/assume_aligned). *Task: tell the compiler a pointer is `alignas(32)`-aligned and observe the codegen difference (optional/system-dependent).*

### P. String utilities

69. **`std::string::starts_with` / `ends_with`** — [spec](https://en.cppreference.com/cpp/string/basic_string/starts_with). *Task: filter a vector of paths to those ending in `.cpp`.*
70. **`std::string_view::starts_with` / `ends_with`** — same spec. *Task: same as 69 but with `string_view`.*

### Q. Algorithms and numeric

71. **`std::midpoint` and `std::lerp`** — [spec](https://en.cppreference.com/cpp/numeric/midpoint). *Task: midpoint of two `int`s without overflow; lerp between two `double`s.*
72. **`std::ssize`** — [spec](https://en.cppreference.com/cpp/iterator/size). *Task: use `std::ssize(v)` to silence a signed/unsigned comparison warning.*
73. **`constexpr` for `<algorithm>`, `<utility>`, `<complex>`** — [spec](https://en.cppreference.com/cpp/algorithm). *Task: `static_assert(std::is_sorted(arr, arr+N));` where `arr` is a `constexpr` array.*

### R. Containers and erase

74. **Uniform container erasure: `std::erase` / `std::erase_if`** — [spec](https://en.cppreference.com/cpp/container/vector/erase2). *Task: remove all negative ints from a `vector<int>` with one call.*
75. **Heterogeneous lookup in unordered containers** — [spec](https://en.cppreference.com/cpp/container/unordered_map). *Task: look up by `std::string_view` in an `unordered_map<std::string, V>` with the right `Hash`/`KeyEqual`.*

### S. Type traits and utilities

76. **`std::remove_cvref` and `remove_cvref_t`** — [spec](https://en.cppreference.com/cpp/types/remove_cvref). *Task: replace `decay_t` with `remove_cvref_t` where decay-to-pointer was a bug.*
77. **`std::is_bounded_array` / `is_unbounded_array`** — [spec](https://en.cppreference.com/cpp/types/is_bounded_array). *Task: distinguish `int[5]` from `int[]`.*
78. **`std::bind_front`** — [spec](https://en.cppreference.com/cpp/utility/functional/bind_front). *Task: prefer `bind_front(&T::foo, this)` over `bind(...)` with placeholders.*

### T. Numbers

79. **Mathematical constants in `<numbers>` (`pi`, `e`, `phi`, `sqrt2`)** — [spec](https://en.cppreference.com/cpp/numeric/constants). *Task: replace any `3.14159...` literals in earlier code with `std::numbers::pi_v<T>`.*

### U. Polymorphic allocator additions

80. **`std::pmr::polymorphic_allocator` member function additions** — [spec](https://en.cppreference.com/cpp/memory/polymorphic_allocator). *Task: explore `new_object` / `delete_object` / `allocate_bytes` on a pmr allocator.*

### V. Execution policies

81. **`std::execution::unseq`** — [spec](https://en.cppreference.com/cpp/algorithm/execution_policy_tag). *Task: use `unseq` (vectorization, single-thread) on a hot loop and compare against `seq`.*
