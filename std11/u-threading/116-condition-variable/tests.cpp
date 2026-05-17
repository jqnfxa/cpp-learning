#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>

#include "solution.hpp"

TEST(condition_variable, single_thread_push_pop)
{
    task116::bounded_buffer buf(4);
    buf.push(1);
    buf.push(2);
    buf.push(3);
    EXPECT_EQ(buf.size(), 3u);
    EXPECT_EQ(buf.pop(), 1);
    EXPECT_EQ(buf.pop(), 2);
    EXPECT_EQ(buf.pop(), 3);
    EXPECT_EQ(buf.size(), 0u);
}

TEST(condition_variable, producer_consumer_order_preserved)
{
    /* The headline test. Consumer starts first and immediately tries
     * to pop. With proper cv.wait, it blocks. With the stub, it
     * accumulates -1 sentinels until the producer catches up. */
    task116::bounded_buffer buf(4);
    constexpr int N = 100;
    std::vector<int> consumed;
    consumed.reserve(N);

    std::thread consumer([&]
                         {
        for (int i = 0; i < N; ++i)
        {
            consumed.push_back(buf.pop());
        } });

    /* Make sure the consumer starts before the producer has anything. */
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    for (int i = 0; i < N; ++i)
    {
        buf.push(i);
    }
    consumer.join();

    ASSERT_EQ(consumed.size(), static_cast<std::size_t>(N));
    /* No sentinels -- proper blocking pop. */
    int sentinels = static_cast<int>(std::count(consumed.begin(), consumed.end(), -1));
    EXPECT_EQ(sentinels, 0) << "pop returned the -1 sentinel; needs cv.wait";
    /* Order preserved. */
    for (int i = 0; i < N; ++i)
    {
        EXPECT_EQ(consumed[static_cast<std::size_t>(i)], i) << "i=" << i;
    }
}

TEST(condition_variable, push_blocks_when_full)
{
    /* Producer pushes faster than consumer pops. With proper
     * cv.wait, the producer blocks when buffer is full. With the
     * stub, the producer overshoots capacity. We check buffer never
     * exceeds the cap during the run via repeated size() probes. */
    task116::bounded_buffer buf(4);
    constexpr int N = 200;

    std::size_t max_observed = 0;
    std::thread sizer([&]
                      {
        for (int i = 0; i < 100; ++i)
        {
            max_observed = std::max(max_observed, buf.size());
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        } });

    std::thread producer([&]
                         {
        for (int i = 0; i < N; ++i)
        {
            buf.push(i);
        } });

    std::thread consumer([&]
                         {
        for (int i = 0; i < N; ++i)
        {
            buf.pop();
        } });

    producer.join();
    consumer.join();
    sizer.join();

    EXPECT_LE(max_observed, 4u)
        << "buffer size exceeded capacity -- push should have blocked";
}

TEST(condition_variable, sum_preserved_over_transfer)
{
    /* Producer pushes 1..1000 (sum 500500). Consumer accumulates
     * the sum. Must equal 500500 with no sentinels. */
    task116::bounded_buffer buf(8);
    constexpr int N = 1000;

    long long sum = 0;
    std::thread consumer([&]
                         {
        for (int i = 0; i < N; ++i)
        {
            sum += buf.pop();
        } });

    /* Consumer pre-empts; give it a moment. */
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    for (int i = 1; i <= N; ++i)
    {
        buf.push(i);
    }
    consumer.join();

    constexpr long long expected = static_cast<long long>(N) * (N + 1) / 2;
    EXPECT_EQ(sum, expected);
}
