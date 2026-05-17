#include <gtest/gtest.h>

#include <atomic>
#include <thread>
#include <vector>

#include "solution.hpp"

namespace
{

class thread_local_test : public ::testing::Test
{
protected:
    void SetUp() override
    {
        task65::reset();
    }
};

}

TEST_F(thread_local_test, main_thread_increments)
{
    EXPECT_EQ(task65::read(), 0);
    EXPECT_EQ(task65::increment(), 1);
    EXPECT_EQ(task65::increment(), 2);
    EXPECT_EQ(task65::increment(), 3);
    EXPECT_EQ(task65::read(), 3);
}

TEST_F(thread_local_test, worker_thread_has_independent_counter)
{
    task65::increment();
    task65::increment();
    task65::increment();
    EXPECT_EQ(task65::read(), 3);

    int worker_start = -1;
    int worker_after = -1;
    std::thread t(
        [&]
        {
            worker_start = task65::read();
            task65::increment();
            task65::increment();
            worker_after = task65::read();
        });
    t.join();

    EXPECT_EQ(worker_start, 0);
    EXPECT_EQ(worker_after, 2);
    EXPECT_EQ(task65::read(), 3); /* main is unaffected. */
}

TEST_F(thread_local_test, many_threads_each_see_their_own_count)
{
    constexpr int N = 32;
    constexpr int per_thread_increments = 1000;

    std::vector<int> observed(N, -1);
    std::vector<std::thread> threads;
    threads.reserve(N);

    for (int i = 0; i < N; ++i)
    {
        threads.emplace_back(
            [i, &observed]
            {
                for (int k = 0; k < per_thread_increments; ++k)
                {
                    task65::increment();
                }
                observed[i] = task65::read();
            });
    }

    for (auto &t : threads)
    {
        t.join();
    }

    for (int i = 0; i < N; ++i)
    {
        EXPECT_EQ(observed[i], per_thread_increments) << "thread " << i;
    }
    /* Main thread is still untouched by any worker. */
    EXPECT_EQ(task65::read(), 0);
}

TEST_F(thread_local_test, addresses_differ_between_threads)
{
    int *main_addr = &task65::counter();

    int *worker_addr = nullptr;
    std::thread t(
        [&]
        {
            worker_addr = &task65::counter();
        });
    t.join();

    ASSERT_NE(worker_addr, nullptr);
    EXPECT_NE(main_addr, worker_addr);
}
