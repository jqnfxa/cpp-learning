#include <gtest/gtest.h>

#include <atomic>
#include <stdexcept>
#include <string>
#include <thread>

#include "solution.hpp"

TEST(exception_ptr, no_throw_returns_normally)
{
    bool ran = false;
    task96::run_and_propagate([&]
                              { ran = true; });
    EXPECT_TRUE(ran);
}

TEST(exception_ptr, propagates_runtime_error)
{
    EXPECT_THROW(task96::run_and_propagate([]
                                           { throw std::runtime_error("oops"); }),
                 std::runtime_error);
}

TEST(exception_ptr, preserves_what_string)
{
    try
    {
        task96::run_and_propagate([]
                                  { throw std::runtime_error("worker said hi"); });
        FAIL() << "expected the worker's exception to surface here";
    }
    catch (const std::runtime_error &e)
    {
        EXPECT_STREQ(e.what(), "worker said hi");
    }
}

TEST(exception_ptr, preserves_concrete_type_through_base_catch)
{
    /* Throw a derived type from the worker; the main thread catches
     * a more general base type. The exception keeps its dynamic
     * type, so a re-catch of the derived type would also work. */
    try
    {
        task96::run_and_propagate([]
                                  { throw std::out_of_range("range"); });
        FAIL() << "expected an exception";
    }
    catch (const std::out_of_range &)
    {
        SUCCEED();
    }
    catch (const std::exception &)
    {
        FAIL() << "caught at the wrong (too-general) level";
    }
}

TEST(exception_ptr, no_throw_means_no_lingering_exception_ptr)
{
    /* Run twice; each invocation is independent. If the second call
     * accidentally re-raised the first's exception, this test would
     * throw. */
    task96::run_and_propagate([] {});
    task96::run_and_propagate([] {});
    SUCCEED();
}

TEST(exception_ptr, worker_actually_ran_on_a_different_thread)
{
    /* Capture the thread::id from inside the lambda; assert it's not
     * this thread. This proves the work didn't accidentally happen
     * on the calling thread (e.g. by an inlined fallback). */
    std::thread::id main_id = std::this_thread::get_id();
    std::thread::id worker_id;

    task96::run_and_propagate([&]
                              { worker_id = std::this_thread::get_id(); });

    EXPECT_NE(worker_id, std::thread::id{});
    EXPECT_NE(worker_id, main_id);
}

TEST(exception_ptr, does_not_call_terminate_on_thrown_exception)
{
    /* If run_and_propagate didn't catch the worker exception, the
     * thread function would unwind out of the thread, hitting
     * std::terminate. The test ever returning at all proves that
     * scenario didn't happen. */
    bool caught = false;
    try
    {
        task96::run_and_propagate([]
                                  { throw std::logic_error("nope"); });
    }
    catch (const std::logic_error &)
    {
        caught = true;
    }
    EXPECT_TRUE(caught);
}

TEST(exception_ptr, custom_string_exception)
{
    /* Custom string exception just to vary the dynamic types in
     * play. The capture/rethrow mechanism shouldn't care. */
    try
    {
        task96::run_and_propagate([]
                                  { throw std::string("a plain string, thrown as-is"); });
        FAIL();
    }
    catch (const std::string &s)
    {
        EXPECT_EQ(s, "a plain string, thrown as-is");
    }
}
