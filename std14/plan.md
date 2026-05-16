# C++14 — step-by-step plan

C++14 is a small, focused release that polishes C++11. Most steps below
unlock a feature you almost-had in C++11. Prerequisite: finish std11/.

Per-task layout (same as std11/):

    std14/<section>/<NN-name>/
        task.md       — exercise spec
        solution.hpp  — you write this
        tests.cpp     — pre-written tests (GoogleTest)
        verify.sh     — build + run tests
        CMakeLists.txt

---

## Part I — Language

### A. `constexpr` gets real

1. **Relaxed `constexpr` functions (multiple statements, local vars, `if`, loops)** — [spec](https://en.cppreference.com/cpp/language/constexpr). *Task: rewrite the C++11 single-return `factorial` from std11 step 39 using an iterative loop inside a `constexpr` function.*
2. **`constexpr` member functions are no longer implicitly `const`** — same spec. *Task: write a `constexpr` `set_x(int)` on a literal `point` type.*

### B. Return-type deduction

3. **Return-type deduction for functions: `auto f(...)`** — [spec](https://en.cppreference.com/cpp/language/function#Return_type_deduction). *Task: rewrite the C++11 trailing-return-type `add` (std11 step 61) as `auto add(A a, B b)`.*
4. **`decltype(auto)`** — [spec](https://en.cppreference.com/cpp/language/auto#decltype.28auto.29). *Task: write `forward_get(x)` that returns `decltype(auto)`; show how it differs from `auto` when `x` is a reference.*

### C. Lambdas continued

5. **Generic lambdas (`auto` params)** — [spec](https://en.cppreference.com/cpp/language/lambda). *Task: write a generic `print = [](auto const &x) { std::cout << x; };` and call it on `int`, `double`, `std::string`.*
6. **Init-capture (capture by move / by computed expression)** — same spec. *Task: capture a `std::unique_ptr` into a lambda using `[p = std::move(p)]`.*

### D. Templates and aggregates

7. **Variable templates** — [spec](https://en.cppreference.com/cpp/language/variable_template). *Task: define `template<class T> constexpr T pi = T(3.14159265358979);` and use it for `float`, `double`, `long double`.*
8. **Aggregates with default member initializers** — [spec](https://en.cppreference.com/cpp/language/aggregate_initialization). *Task: `struct point { int x = 0; int y = 0; };` — construct via `{}`, `{1}`, `{1, 2}`.*

### E. Allocation control

9. **New/delete elision (compiler may fuse allocations)** — [spec](https://en.cppreference.com/cpp/language/new#Allocation). *Task: read-only; in a comment, explain when the compiler may elide an `operator new` call.*

### F. Literals

10. **Binary literals (`0b...`)** — [spec](https://en.cppreference.com/cpp/language/integer_literal). *Task: define `constexpr auto mask = 0b1010'0011;`.*
11. **Digit separators (`'`)** — same spec. *Task: write `constexpr auto million = 1'000'000;`.*

---

## Part II — Library

### G. Smart pointers and utilities

12. **`std::make_unique`** — [spec](https://en.cppreference.com/cpp/memory/make_unique). *Task: replace every `unique_ptr<T>(new T(...))` in std11 step 67 with `make_unique<T>(...)`.*
13. **`std::exchange`** — [spec](https://en.cppreference.com/cpp/utility/exchange). *Task: rewrite a move ctor to use `exchange(other.ptr_, nullptr)` instead of explicit assignment.*

### H. Compile-time toolkit

14. **`std::integer_sequence` and `std::index_sequence`** — [spec](https://en.cppreference.com/cpp/utility/integer_sequence). *Task: unpack a tuple into a function call using `std::index_sequence_for<Args...>`.*

### I. Concurrency

15. **`std::shared_timed_mutex`** — [spec](https://en.cppreference.com/cpp/thread/shared_timed_mutex). *Task: many readers, one writer protecting a map.*
16. **`std::shared_lock`** — [spec](https://en.cppreference.com/cpp/thread/shared_lock). *Task: use `shared_lock` for readers and `unique_lock` for writers on the map from step 15.*

### J. I/O and time

17. **`std::quoted`** — [spec](https://en.cppreference.com/cpp/io/manip/quoted). *Task: round-trip a `std::string` containing spaces and quotes through a `stringstream`.*
18. **`chrono` UDLs: `s`, `ms`, `us`, `ns`, `min`, `h`** — [spec](https://en.cppreference.com/cpp/chrono/operator%22%22s). *Task: `using namespace std::chrono_literals;` then `std::this_thread::sleep_for(50ms);`.*
19. **`string` UDL: `""s`** — [spec](https://en.cppreference.com/cpp/string/basic_string/operator%22%22s). *Task: `"hello"s + " world"` (returns `std::string`, not `const char *`).*
20. **`complex` UDLs: `i`, `if`, `il`** — [spec](https://en.cppreference.com/cpp/numeric/complex/operator%22%22i). *Task: write `(1.0 + 2i)` and print its modulus.*

### K. Type traits — alias forms

21. **`_t` alias forms of type traits (`remove_cv_t`, `decay_t`, `enable_if_t`, ...)** — [spec](https://en.cppreference.com/cpp/header/type_traits). *Task: rewrite a few of std11's `typename std::remove_cv<T>::type` sites as `std::remove_cv_t<T>`.*

### L. Algorithm tweaks

22. **Two-range overloads (`std::equal`, `std::mismatch`, `std::is_permutation`)** — [spec](https://en.cppreference.com/cpp/algorithm/equal). *Task: compare two vectors of different sizes safely using the two-range overload.*

### M. Tuple addressing

23. **`std::get<T>(tuple)` (by type instead of index)** — [spec](https://en.cppreference.com/cpp/utility/tuple/get). *Task: build a `tuple<int, std::string, double>` and retrieve fields by their type.*
