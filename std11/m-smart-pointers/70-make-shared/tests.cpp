#include <gtest/gtest.h>

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <new>

#include "solution.hpp"

/*
 * Global operator new / delete replacements with an opt-in counter.
 * Gtest itself allocates plenty; we only count during a narrow scope
 * controlled by the `counting` flag.
 */

namespace
{

std::size_t g_alloc_count = 0;
bool g_counting = false;

}

void *operator new(std::size_t n)
{
    void *p = std::malloc(n);
    if (!p)
    {
        throw std::bad_alloc();
    }
    if (g_counting)
    {
        ++g_alloc_count;
    }
    return p;
}

void operator delete(void *p) noexcept
{
    std::free(p);
}

void operator delete(void *p, std::size_t) noexcept
{
    std::free(p);
}

namespace
{

template <class Fn>
std::size_t count_allocations(Fn &&fn)
{
    g_alloc_count = 0;
    g_counting = true;
    fn();
    g_counting = false;
    return g_alloc_count;
}

}

TEST(make_shared, make_via_new_returns_owning_pointer)
{
    auto p = task70::make_via_new(42);
    ASSERT_NE(p.get(), nullptr);
    EXPECT_EQ(*p, 42);
    EXPECT_EQ(p.use_count(), 1);
}

TEST(make_shared, make_via_make_shared_returns_owning_pointer)
{
    auto p = task70::make_via_make_shared(42);
    ASSERT_NE(p.get(), nullptr);
    EXPECT_EQ(*p, 42);
    EXPECT_EQ(p.use_count(), 1);
}

TEST(make_shared, make_via_new_does_two_allocations)
{
    auto n = count_allocations([]
                               { auto p = task70::make_via_new(42); (void)p; });
    EXPECT_EQ(n, 2u) << "shared_ptr<T>(new T(...)) is object + control block";
}

TEST(make_shared, make_via_make_shared_does_one_allocation)
{
    auto n = count_allocations([]
                               { auto p = task70::make_via_make_shared(42); (void)p; });
    EXPECT_EQ(n, 1u) << "make_shared collapses object + control block into one alloc";
}

TEST(make_shared, make_point_forwards_constructor_args)
{
    auto p = task70::make_point(3, 4);
    ASSERT_NE(p.get(), nullptr);
    EXPECT_EQ(p->x, 3);
    EXPECT_EQ(p->y, 4);
    EXPECT_EQ(p.use_count(), 1);
}

TEST(make_shared, make_point_does_one_allocation)
{
    auto n = count_allocations([]
                               { auto p = task70::make_point(1, 2); (void)p; });
    EXPECT_EQ(n, 1u);
}
