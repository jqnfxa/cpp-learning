# C++11 — step-by-step plan

Pedagogical order. Each step assumes you've done the ones above. Hot
topics (lambdas, moves, templates, threading, chrono, smart pointers)
are split into sub-steps so each task stays small.

Per-task layout (created as you reach each step):

    std11/<section>/<NN-name>/
        task.md       — exercise spec
        solution.hpp  — you write this
        tests.cpp     — pre-written tests (GoogleTest)
        verify.sh     — build + run tests
        CMakeLists.txt

---

## Part I — Language

### A. Foundations (types, literals, syntax)

1. **`nullptr` and `nullptr_t`** — [spec](https://en.cppreference.com/cpp/language/nullptr). *Task: overload `f(int)` and `f(int*)`; show `f(nullptr)` picks the pointer overload while `f(0)` picks the int one.*
2. **`long long`** — [spec](https://en.cppreference.com/cpp/language/types). *Task: print `sizeof(long long)`, write `mul64(a, b)` that returns `long long`.*
3. **`char16_t` and `char32_t`** — [spec](https://en.cppreference.com/cpp/language/types). *Task: write a function that counts UTF-16 code units in a `u"..."` literal.*
4. **Unicode string literals: `u8"…"`, `u"…"`, `U"…"`, `R"(…)"`** — [spec](https://en.cppreference.com/cpp/language/string_literal). *Task: round-trip a string through `u8`, `u`, `U`, `R` literals and confirm `sizeof` per code unit.*
5. **`static_assert`** — [spec](https://en.cppreference.com/cpp/language/static_assert). *Task: write a constexpr `is_power_of_two(n)` and assert it on several constants.*
6. **`alignof`** — [spec](https://en.cppreference.com/cpp/language/alignof). *Task: print alignment of `char`, `int`, `double`, `long double`, and a packed-looking struct.*
7. **`alignas`** — [spec](https://en.cppreference.com/cpp/language/alignas). *Task: declare an `alignas(64)` cache-line buffer and assert its address.*
8. **Type aliases with `using`** — [spec](https://en.cppreference.com/cpp/language/type_alias). *Task: replace a `typedef` chain with `using` aliases.*
9. **Alias templates** — [spec](https://en.cppreference.com/cpp/language/type_alias). *Task: `template<class T> using vec = std::vector<T>;` and use it.*
10. **Attributes: `[[noreturn]]`** — [spec](https://en.cppreference.com/cpp/language/attributes/noreturn). *Task: mark a `panic(msg)` that throws as `[[noreturn]]` and silence the unreachable-warning at the call site.*
11. **Attributes: `[[carries_dependency]]`** — [spec](https://en.cppreference.com/cpp/language/attributes/carries_dependency). *Task: read-only; explain in a comment when it changes codegen on weakly ordered hardware.*

### B. Scoped enums

12. **`enum class`** — [spec](https://en.cppreference.com/cpp/language/enum). *Task: define `enum class color { red, green, blue };` and a `to_string(color)` switch.*
13. **`enum class` with explicit underlying type** — [spec](https://en.cppreference.com/cpp/language/enum). *Task: `enum class status : std::uint8_t { ok, fail }` and assert its size.*
14. **Forward-declared enums** — [spec](https://en.cppreference.com/cpp/language/enum). *Task: forward-declare an enum in a header and define it in a `.cpp`.*

### C. Range-based for

15. **`range-for` over arrays** — [spec](https://en.cppreference.com/cpp/language/range-for). *Task: sum a C array with range-for.*
16. **`range-for` over containers** — same spec. *Task: print a `std::vector<int>` element by element.*
17. **`range-for` with `auto &` vs `auto const &` vs `auto`** — same spec. *Task: write each variant and show which form mutates and which copies.*

### D. Initialization

18. **List initialization (uniform `T{}`)** — [spec](https://en.cppreference.com/cpp/language/list_initialization). *Task: initialize `int`, `std::pair`, `std::vector<int>` all with `T{...}`.*
19. **Narrowing prevention with `{}`** — same spec. *Task: show `int x{3.5};` fails to compile while `int x(3.5);` silently truncates.*
20. **`std::initializer_list`** — [spec](https://en.cppreference.com/cpp/utility/initializer_list). *Task: write a `sum({1,2,3})` taking `std::initializer_list<int>`.*
21. **Brace-or-equal NSDMIs** — [spec](https://en.cppreference.com/cpp/language/data_members#Member_initialization). *Task: `struct s { int x = 7; std::string name{"default"}; };` — write and test.*
22. **Delegating constructors** — [spec](https://en.cppreference.com/cpp/language/constructor#Delegating_constructor). *Task: a `point` with `point()` delegating to `point(0, 0)`.*
23. **Inheriting constructors with `using Base::Base;`** — [spec](https://en.cppreference.com/cpp/language/using_declaration#Inheriting_constructors). *Task: a `derived` that re-uses every `base` ctor.*

### E. Special member function control

24. **`= default`** — [spec](https://en.cppreference.com/cpp/language/function#Function_definition). *Task: define a class with explicit `= default` for the default ctor and copy ops.*
25. **`= delete`** — same spec. *Task: forbid copying on a `noncopyable` type using `= delete`.*
26. **`final` on classes** — [spec](https://en.cppreference.com/cpp/language/final). *Task: mark a class `final` and show inheritance fails.*
27. **`final` on virtual functions** — same spec. *Task: in a 3-level hierarchy mark a middle override `final` and show the leaf cannot override it.*
28. **`override`** — [spec](https://en.cppreference.com/cpp/language/override). *Task: introduce a typo'd override; show `override` catches it.*

### F. Value categories and moves

29. **lvalue / rvalue / xvalue / prvalue — what they are** — [spec](https://en.cppreference.com/cpp/language/value_category). *Task: classify every expression in a given snippet.*
30. **Rvalue references (`T &&`)** — [spec](https://en.cppreference.com/cpp/language/reference). *Task: write `f(T &)` and `f(T &&)` overloads, show which is picked for `f(x)` vs `f(std::move(x))`.*
31. **`std::move`** — [spec](https://en.cppreference.com/cpp/utility/move). *Task: write a `swap(a, b)` using `std::move`, no temporaries other than the move target.*
32. **Move constructor** — [spec](https://en.cppreference.com/cpp/language/move_constructor). *Task: write a `buffer` owning a `std::unique_ptr<int[]>` with a move ctor.*
33. **Move assignment operator** — [spec](https://en.cppreference.com/cpp/language/move_assignment). *Task: extend the `buffer` from step 32 with a move-assign that handles self-move.*
34. **Rule of five** — [spec](https://en.cppreference.com/cpp/language/rule_of_three). *Task: explain (in a comment) when defining one of {dtor, copy, move} forces you to define the others.*
35. **`noexcept` move ops and why containers care** — [spec](https://en.cppreference.com/cpp/language/noexcept_spec). *Task: write a move ctor with and without `noexcept`; observe the difference in `std::vector<T>::push_back` behavior.*
36. **`std::forward` and perfect forwarding** — [spec](https://en.cppreference.com/cpp/utility/forward). *Task: write a `factory<T>(Args &&...)` that perfect-forwards.*
37. **Reference collapsing** — [spec](https://en.cppreference.com/cpp/language/reference#Reference_collapsing). *Task: in a forwarding-reference template, demonstrate `T && &` → `T &`, `T && &&` → `T &&`.*

### G. `constexpr` & literal types

38. **`constexpr` variables** — [spec](https://en.cppreference.com/cpp/language/constexpr). *Task: define `constexpr int kSize = 16;` and use it as an array bound.*
39. **`constexpr` functions (C++11 strict: single return)** — same spec. *Task: write a `constexpr int factorial(int n)` using the ternary operator and recursion.*
40. **Literal types and constexpr constructors** — [spec](https://en.cppreference.com/cpp/named_req/LiteralType). *Task: write a `constexpr point{x, y}` usable in a constexpr context.*
41. **User-defined literals: integer** — [spec](https://en.cppreference.com/cpp/language/user_literal). *Task: implement `operator"" _km` returning meters.*
42. **User-defined literals: floating-point** — same spec. *Task: implement `operator"" _deg` returning radians.*
43. **User-defined literals: string** — same spec. *Task: implement `operator"" _bytes` returning a `std::vector<std::byte>`-equivalent (use `std::vector<unsigned char>` here).*
44. **User-defined literals: raw and template forms** — same spec. *Task: write a numeric raw literal `operator"" _bin(const char *)` that parses `0`/`1`.*

### H. `noexcept`

45. **`noexcept` specifier** — [spec](https://en.cppreference.com/cpp/language/noexcept_spec). *Task: mark a `clone()` `noexcept` and demonstrate compile-time enforcement on call sites.*
46. **`noexcept` operator** — [spec](https://en.cppreference.com/cpp/language/noexcept). *Task: write `static_assert(noexcept(f()))` over a noexcept and a throwing function.*
47. **Conditional `noexcept`** — same spec. *Task: write `swap(a, b) noexcept(noexcept(...))` that propagates correctly.*

### I. Lambda expressions

48. **Lambdas: basic syntax and call** — [spec](https://en.cppreference.com/cpp/language/lambda). *Task: write `auto inc = [](int x) { return x + 1; };` and call it.*
49. **Captures by value `[x]`** — same spec. *Task: capture an `int` by value, mutate it inside, show outer copy is unchanged.*
50. **Captures by reference `[&x]`** — same spec. *Task: capture by reference, increment from the lambda, show the outer changed.*
51. **Default captures `[=]` and `[&]`** — same spec. *Task: write a lambda that captures all locals by value vs by reference.*
52. **`mutable` lambdas** — same spec. *Task: capture `[count = 0]`-style isn't C++11 yet — capture by value, mark the lambda `mutable`, and increment.*
53. **Captureless lambda → function pointer** — same spec. *Task: assign `[](int x) { return x*2; }` to `int(*)(int)` and call through it.*
54. **Lambda as a `std::function`** — [spec](https://en.cppreference.com/cpp/utility/functional/function). *Task: store a capturing lambda in `std::function<int(int)>`.*

### J. Templates

55. **Variadic templates: parameter packs** — [spec](https://en.cppreference.com/cpp/language/parameter_pack). *Task: write `template<class... Ts> void print(Ts... args);` that prints each.*
56. **Pack expansion** — same spec. *Task: write `min_of(a, b, c, ...)` using pack expansion in an initializer list.*
57. **`sizeof...(Pack)`** — same spec. *Task: write `count_args(args...)` returning `sizeof...(args)`.*
58. **Variadic template recursion** — same spec. *Task: implement `print(...)` via a recursive head/tail split.*
59. **Forwarding references in templates** — [spec](https://en.cppreference.com/cpp/language/reference#Forwarding_references). *Task: write `wrap(T &&t)` that prints whether `t` is lvalue or rvalue.*

### K. Other type-system upgrades

60. **`auto` for variables** — [spec](https://en.cppreference.com/cpp/language/auto). *Task: rewrite a function full of explicit iterator types using `auto`.*
61. **Trailing return type `-> T`** — [spec](https://en.cppreference.com/cpp/language/function#Function_declaration). *Task: write `template<class A, class B> auto add(A a, B b) -> decltype(a+b);`.*
62. **`decltype`** — [spec](https://en.cppreference.com/cpp/language/decltype). *Task: write `template<class T> using element_t = decltype(*std::declval<T>().begin());`.*
63. **Generalized (non-trivial) unions** — [spec](https://en.cppreference.com/cpp/language/union). *Task: a discriminated `int_or_string` union with placement-new in each branch.*
64. **Trivial and standard-layout types** — [spec](https://en.cppreference.com/cpp/named_req/TrivialType), [spec](https://en.cppreference.com/cpp/named_req/StandardLayoutType). *Task: write `static_assert`s confirming a POD-ish struct is trivial and standard-layout, then make it non-trivial.*

### L. Concurrency primitives (language side)

65. **Thread-local storage** — [spec](https://en.cppreference.com/cpp/language/storage_duration#Thread_storage_duration). *Task: declare a `thread_local int counter;` and prove threads see independent values.*
66. **The C++11 memory model (conceptual)** — [spec](https://en.cppreference.com/cpp/atomic/memory_order). *Task: read the spec; classify load/store pairs in a snippet as data race / not.*

---

## Part II — Library

### M. Smart pointers

67. **`std::unique_ptr<T>`** — [spec](https://en.cppreference.com/cpp/memory/unique_ptr). *Task: own a heap `int[]` via `unique_ptr<int[]>`; assert no leak on early return.*
68. **`std::unique_ptr` with a custom deleter** — same spec. *Task: wrap a `FILE *` with a `unique_ptr<FILE, decltype(&std::fclose)>`.*
69. **`std::shared_ptr<T>`** — [spec](https://en.cppreference.com/cpp/memory/shared_ptr). *Task: share ownership between two functions; print `use_count()`.*
70. **`std::make_shared`** — [spec](https://en.cppreference.com/cpp/memory/make_shared). *Task: replace `shared_ptr<T>(new T(...))` with `make_shared<T>(...)`; explain the allocation difference in a comment.*
71. **`std::weak_ptr<T>`** — [spec](https://en.cppreference.com/cpp/memory/weak_ptr). *Task: build a `parent -> child` `shared_ptr` and a `child -> parent` `weak_ptr` to avoid a cycle.*
72. **`std::enable_shared_from_this`** — [spec](https://en.cppreference.com/cpp/memory/enable_shared_from_this). *Task: a class that hands out a `shared_ptr` to itself safely.*

### N. Containers and `emplace`

73. **`std::array<T, N>`** — [spec](https://en.cppreference.com/cpp/container/array). *Task: write a `mean()` over `std::array<double, 4>`.*
74. **`std::forward_list<T>`** — [spec](https://en.cppreference.com/cpp/container/forward_list). *Task: insert in the middle using `insert_after`.*
75. **`std::unordered_map<K, V>`** — [spec](https://en.cppreference.com/cpp/container/unordered_map). *Task: word-count from a vector of strings.*
76. **`std::unordered_set<T>`** — [spec](https://en.cppreference.com/cpp/container/unordered_set). *Task: deduplicate a vector preserving first-seen order.*
77. **`emplace` / `emplace_back` / `emplace_front`** — [spec](https://en.cppreference.com/cpp/container/vector/emplace_back). *Task: construct a `vector<std::pair<int,string>>` in place; compare to `push_back({...})`.*
78. **Move into a container** — [spec](https://en.cppreference.com/cpp/container/vector/push_back). *Task: `push_back(std::move(s))` from a local `std::string`; show the local becomes empty.*
79. **`std::move_iterator`** — [spec](https://en.cppreference.com/cpp/iterator/move_iterator). *Task: move-construct a target vector from a source vector using `std::make_move_iterator`.*

### O. Algorithms (new in C++11)

80. **`std::all_of` / `any_of` / `none_of`** — [spec](https://en.cppreference.com/cpp/algorithm/all_any_none_of). *Task: implement `is_all_positive`, `has_zero`, `none_negative` over a vector.*
81. **`std::find_if_not`** — [spec](https://en.cppreference.com/cpp/algorithm/find). *Task: find the first non-whitespace character in a `std::string`.*
82. **`std::copy_if` and `std::copy_n`** — [spec](https://en.cppreference.com/cpp/algorithm/copy). *Task: copy only even numbers; copy first `k` items.*
83. **`std::move` (algorithm) and `std::move_backward`** — [spec](https://en.cppreference.com/cpp/algorithm/move). *Task: move a vector of strings into another, leaving the source empty.*
84. **`std::shuffle`** — [spec](https://en.cppreference.com/cpp/algorithm/random_shuffle). *Task: deal 5 cards from a 52-card deck using a `std::mt19937`.*
85. **`std::is_partitioned` / `partition_copy` / `partition_point`** — [spec](https://en.cppreference.com/cpp/algorithm/is_partitioned). *Task: separate negatives and non-negatives; find the partition point.*
86. **`std::is_sorted` / `is_sorted_until`** — [spec](https://en.cppreference.com/cpp/algorithm/is_sorted). *Task: report the first descent point in a vector.*
87. **`std::is_heap` / `is_heap_until`** — [spec](https://en.cppreference.com/cpp/algorithm/is_heap). *Task: validate a max-heap; identify the first violator.*
88. **`std::minmax` and `std::minmax_element`** — [spec](https://en.cppreference.com/cpp/algorithm/minmax). *Task: find the min and max of a range in one pass.*
89. **`std::is_permutation`** — [spec](https://en.cppreference.com/cpp/algorithm/is_permutation). *Task: detect anagrams.*
90. **`std::iota`** — [spec](https://en.cppreference.com/cpp/algorithm/iota). *Task: fill a `vector<int>(10)` with `0..9`.*
91. **`std::uninitialized_copy_n`** — [spec](https://en.cppreference.com/cpp/memory/uninitialized_copy_n). *Task: copy-construct N objects into a raw buffer.*

### P. Iterator helpers

92. **Free `std::begin` / `std::end`** — [spec](https://en.cppreference.com/cpp/iterator/begin). *Task: write a `print_all(c)` that works for `std::vector`, `std::array`, and a C array.*
93. **`std::next` and `std::prev`** — [spec](https://en.cppreference.com/cpp/iterator/next). *Task: advance by `k` without writing a hand loop.*

### Q. Function objects, errors, exceptions

94. **`std::function`** — [spec](https://en.cppreference.com/cpp/utility/functional/function). *Task: a `dispatch_table` mapping `std::string` to `std::function<int(int)>`.*
95. **`std::bind`** — [spec](https://en.cppreference.com/cpp/utility/functional/bind). *Task: bind a member function to a fixed `this` and a placeholder.*
96. **`std::exception_ptr`, `current_exception`, `rethrow_exception`** — [spec](https://en.cppreference.com/cpp/error/exception_ptr). *Task: capture an exception in a worker thread; rethrow it on the main thread.*
97. **`std::error_code` / `error_condition` / `system_error`** — [spec](https://en.cppreference.com/cpp/error/error_code). *Task: open a missing file via the POSIX API; convert `errno` into a `system_error`.*

### R. `std::tuple` and `std::pair`

98. **`std::tuple<...>` basics** — [spec](https://en.cppreference.com/cpp/utility/tuple). *Task: make and return a `tuple<int, std::string, double>`.*
99. **`std::get<I>(tuple)` and `std::tie`** — [spec](https://en.cppreference.com/cpp/utility/tuple/get). *Task: unpack a tuple with `std::tie`.*
100. **`std::make_tuple` and `std::forward_as_tuple`** — same spec. *Task: forward-as-tuple inputs into an in-place ctor call (`piecewise_construct`).*

### S. Type traits and `<type_traits>`

101. **Property traits: `is_integral`, `is_floating_point`, `is_pointer`** — [spec](https://en.cppreference.com/cpp/header/type_traits). *Task: write `name_kind<T>()` returning a string.*
102. **Transformation traits: `remove_reference`, `remove_cv`, `decay`** — same spec. *Task: write a metafunction `bare_type<T>` and prove it via `static_assert`.*
103. **`std::enable_if` and SFINAE** — [spec](https://en.cppreference.com/cpp/types/enable_if). *Task: write two overloads of `to_int(T)`, one for arithmetic types and one for strings.*
104. **`std::conditional`** — [spec](https://en.cppreference.com/cpp/types/conditional). *Task: define `wider_t<A, B>` returning the larger of two integral types.*
105. **`std::common_type`** — [spec](https://en.cppreference.com/cpp/types/common_type). *Task: write a varadic `max(...)` returning `common_type_t<Args...>`.*

### T. `<chrono>`

106. **`std::chrono::duration`** — [spec](https://en.cppreference.com/cpp/chrono/duration). *Task: declare `5s`, `100ms` (note: literals are C++14; here just use `seconds`/`milliseconds`).*
107. **Duration arithmetic and ratios** — same spec. *Task: convert milliseconds to seconds using `duration_cast`.*
108. **`std::chrono::time_point`** — [spec](https://en.cppreference.com/cpp/chrono/time_point). *Task: snapshot `steady_clock::now()` around a workload; print the elapsed `microseconds`.*
109. **Clocks: `system_clock`, `steady_clock`, `high_resolution_clock`** — [spec](https://en.cppreference.com/cpp/chrono#Clocks). *Task: pick the right clock for each of: timeout, profiling, "current wall time".*
110. **`std::ratio`** — [spec](https://en.cppreference.com/cpp/numeric/ratio). *Task: compute `std::ratio_add<ratio<1,3>, ratio<1,6>>` and confirm = 1/2.*

### U. Threading

111. **`std::thread`** — [spec](https://en.cppreference.com/cpp/thread/thread). *Task: spawn a thread that prints its `get_id()`.*
112. **`join` and `detach`** — same spec. *Task: write a `joiner` RAII wrapper around `std::thread` that joins in its dtor.*
113. **`std::mutex` and `std::lock_guard`** — [spec](https://en.cppreference.com/cpp/thread/mutex), [spec](https://en.cppreference.com/cpp/thread/lock_guard). *Task: protect a shared counter incremented by 4 threads.*
114. **`std::unique_lock`** — [spec](https://en.cppreference.com/cpp/thread/unique_lock). *Task: rewrite step 113 to unlock partway through a function.*
115. **`std::recursive_mutex`** — [spec](https://en.cppreference.com/cpp/thread/recursive_mutex). *Task: a function that grabs the same mutex recursively; show that a plain `mutex` would deadlock.*
116. **`std::condition_variable`** — [spec](https://en.cppreference.com/cpp/thread/condition_variable). *Task: bounded buffer with one producer and one consumer.*
117. **`std::atomic<T>`** — [spec](https://en.cppreference.com/cpp/atomic/atomic). *Task: replace the mutex in step 113 with `atomic<int>` and `fetch_add`.*
118. **Memory orderings** — [spec](https://en.cppreference.com/cpp/atomic/memory_order). *Task: implement a flag-based handoff with `release`/`acquire` and explain why `relaxed` is wrong.*
119. **`std::future` and `std::promise`** — [spec](https://en.cppreference.com/cpp/thread/future), [spec](https://en.cppreference.com/cpp/thread/promise). *Task: a worker thread fulfills a promise; main thread waits on the future.*
120. **`std::async` and launch policies** — [spec](https://en.cppreference.com/cpp/thread/async). *Task: run a function with `launch::async` vs `launch::deferred`; observe when it actually starts.*
121. **`std::packaged_task`** — [spec](https://en.cppreference.com/cpp/thread/packaged_task). *Task: enqueue a task whose result is collected via its future.*

### V. `<random>`

122. **Engines: `std::mt19937` / `std::mt19937_64`** — [spec](https://en.cppreference.com/cpp/numeric/random). *Task: seed an engine from `random_device` and produce 8 ints.*
123. **Distributions: `uniform_int`, `uniform_real`** — same spec. *Task: simulate 1000 dice rolls and report the histogram.*
124. **Distributions: `normal`, `bernoulli`, `poisson`** — same spec. *Task: sample 1000 draws from each; assert sample mean is near the analytic mean.*

### W. `<regex>`

125. **`std::regex` basics** — [spec](https://en.cppreference.com/cpp/regex). *Task: validate an email-shaped string.*
126. **`regex_search` and capture groups** — [spec](https://en.cppreference.com/cpp/regex/regex_search). *Task: extract `host` and `port` from `"host:port"`.*
127. **`regex_iterator`** — [spec](https://en.cppreference.com/cpp/regex/regex_iterator). *Task: tokenize words from a sentence.*
128. **`regex_replace`** — [spec](https://en.cppreference.com/cpp/regex/regex_replace). *Task: redact every digit in a string with `#`.*

### X. Stateful and scoped allocators

129. **Custom allocator (minimal C++11 form)** — [spec](https://en.cppreference.com/cpp/memory/allocator). *Task: write a counting allocator that logs `allocate`/`deallocate` and plug it into `std::vector<int, MyAlloc>`.*
130. **`std::scoped_allocator_adaptor`** — [spec](https://en.cppreference.com/cpp/memory/scoped_allocator_adaptor). *Task: a `vector<vector<int, A>, scoped_allocator_adaptor<A>>` and trace which allocator services the inner.*

### Y. Hashing

131. **`std::hash<T>` specializations** — [spec](https://en.cppreference.com/cpp/utility/hash). *Task: specialize `std::hash<my_struct>` and use it in `std::unordered_set<my_struct>`.*
