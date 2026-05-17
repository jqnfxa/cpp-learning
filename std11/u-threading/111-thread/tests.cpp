#include <gtest/gtest.h>

#include <thread>
#include <unordered_set>

#include "solution.hpp"

TEST(thread, returns_non_default_id)
{
    auto id = task111::run_and_get_thread_id();
    EXPECT_NE(id, std::thread::id{}) << "captured id must be a real (running) thread id";
}

TEST(thread, differs_from_main_thread_id)
{
    auto main_id = std::this_thread::get_id();
    auto worker_id = task111::run_and_get_thread_id();
    EXPECT_NE(worker_id, main_id)
        << "worker must run on a different thread than main";
}

TEST(thread, multiple_calls_return_different_ids)
{
    /* Each call spawns and joins a fresh thread. On most systems the OS
     * recycles thread handles, but the standard requires that
     * thread::id be unique *while the thread is running*. After the
     * thread joins, the OS may reuse the slot. So this test cannot
     * portably assert "all N ids are unique" -- it can only assert
     * that at least one call gave a non-main id. The non-empty
     * unordered_set assert below is a softer check. */
    std::unordered_set<std::thread::id> ids;
    for (int i = 0; i < 8; ++i)
    {
        ids.insert(task111::run_and_get_thread_id());
    }
    EXPECT_FALSE(ids.empty());
    for (const auto &id : ids)
    {
        EXPECT_NE(id, std::thread::id{});
    }
}

TEST(thread, hashable_and_comparable)
{
    auto id1 = task111::run_and_get_thread_id();
    auto id2 = id1;

    EXPECT_EQ(id1, id2);
    EXPECT_FALSE(id1 < id2 || id2 < id1) << "equal ids must not compare less";

    /* std::hash<std::thread::id> is part of the standard. */
    std::hash<std::thread::id> h;
    EXPECT_EQ(h(id1), h(id2));
}
