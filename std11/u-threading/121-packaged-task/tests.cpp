#include <gtest/gtest.h>

#include <future>
#include <stdexcept>

#include "solution.hpp"

TEST(packaged_task, basic_doubling)
{
    auto f = task121::enqueue_task([](int x)
                                   { return x * 2; }, 5);
    EXPECT_EQ(f.get(), 10);
}

TEST(packaged_task, identity)
{
    auto f = task121::enqueue_task([](int x)
                                   { return x; }, 42);
    EXPECT_EQ(f.get(), 42);
}

TEST(packaged_task, negative_input)
{
    auto f = task121::enqueue_task([](int x)
                                   { return -x; }, 7);
    EXPECT_EQ(f.get(), -7);
}

TEST(packaged_task, zero_input)
{
    auto f = task121::enqueue_task([](int x)
                                   { return x * x; }, 0);
    EXPECT_EQ(f.get(), 0);
}

TEST(packaged_task, multiple_independent_tasks)
{
    auto f1 = task121::enqueue_task([](int x)
                                    { return x + 1; }, 10);
    auto f2 = task121::enqueue_task([](int x)
                                    { return x * 3; }, 5);
    auto f3 = task121::enqueue_task([](int x)
                                    { return x - 2; }, 8);
    EXPECT_EQ(f1.get(), 11);
    EXPECT_EQ(f2.get(), 15);
    EXPECT_EQ(f3.get(), 6);
}

TEST(packaged_task, exception_propagates_through_future)
{
    auto f = task121::enqueue_task(
        [](int /*x*/) -> int
        { throw std::runtime_error("task error"); }, 0);
    EXPECT_THROW(f.get(), std::runtime_error);
}

TEST(packaged_task, exception_preserves_what)
{
    auto f = task121::enqueue_task(
        [](int /*x*/) -> int
        { throw std::runtime_error("specific"); }, 0);
    try
    {
        f.get();
        FAIL();
    }
    catch (const std::runtime_error &e)
    {
        EXPECT_STREQ(e.what(), "specific");
    }
}

TEST(packaged_task, batch_returns_in_order)
{
    /* Enqueue 10 tasks; collect all futures; .get() each in order.
     * Each result matches its input transformation. */
    std::vector<std::future<int>> futures;
    futures.reserve(10);
    for (int i = 0; i < 10; ++i)
    {
        futures.push_back(task121::enqueue_task([](int x)
                                                { return x * 10; }, i));
    }
    for (int i = 0; i < 10; ++i)
    {
        EXPECT_EQ(futures[static_cast<std::size_t>(i)].get(), i * 10) << "i=" << i;
    }
}
