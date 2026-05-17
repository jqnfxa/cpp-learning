#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <utility>

#include "solution.hpp"

TEST(joiner, constructed_joiner_is_joinable)
{
    task112::joiner j(std::thread([]
                                  { std::this_thread::sleep_for(std::chrono::milliseconds(5)); }));
    EXPECT_TRUE(j.joinable());
}

TEST(joiner, get_id_returns_worker_id)
{
    auto captured = std::make_shared<std::thread::id>();
    task112::joiner j(std::thread([captured]
                                  { *captured = std::this_thread::get_id(); }));
    auto observed = j.get_id();
    /* j's destructor will join; after that the captured id is final.
     * For the observed id, we just check it's a real id (not the
     * default-constructed sentinel). */
    EXPECT_NE(observed, std::thread::id{});
}

TEST(joiner, dtor_blocks_until_thread_finishes)
{
    /* This is the headline test: with the stub's detach in the dtor,
     * the destructor returns immediately and `done` is still false.
     * With the reference's join, the destructor waits the full 50ms,
     * the thread sets `done`, and the assertion passes.
     *
     * We use shared_ptr<atomic<bool>> so the storage outlives a
     * detached thread, avoiding UB and keeping the stub's failure
     * mode clean (failed assertion rather than crash). */
    auto done = std::make_shared<std::atomic<bool>>(false);
    {
        task112::joiner j(std::thread([done]
                                      {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            done->store(true); }));
    } /* ~joiner -- should join here */
    EXPECT_TRUE(done->load())
        << "joiner's destructor must block until the thread finishes";
}

TEST(joiner, move_construction_transfers_thread)
{
    task112::joiner src(std::thread([]
                                    { std::this_thread::sleep_for(std::chrono::milliseconds(5)); }));
    EXPECT_TRUE(src.joinable());

    task112::joiner dst(std::move(src));
    EXPECT_FALSE(src.joinable()) << "moved-from joiner should not be joinable";
    EXPECT_TRUE(dst.joinable());
}

TEST(joiner, no_terminate_after_destruction)
{
    /* If the destructor failed to join AND failed to detach a joinable
     * thread, std::terminate would fire. The test running at all to
     * reach EXPECT_TRUE means the destructor disposed of the thread
     * one way or another. */
    {
        task112::joiner j(std::thread([]
                                      { std::this_thread::sleep_for(std::chrono::milliseconds(5)); }));
    }
    EXPECT_TRUE(true); /* survived ~joiner */
}

TEST(joiner, multiple_joiners_in_sequence)
{
    auto counter = std::make_shared<std::atomic<int>>(0);
    {
        task112::joiner a(std::thread([counter]
                                      {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            counter->fetch_add(1); }));
        task112::joiner b(std::thread([counter]
                                      {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            counter->fetch_add(1); }));
    } /* both joiners destruct in reverse order; both must join */
    EXPECT_EQ(counter->load(), 2);
}
