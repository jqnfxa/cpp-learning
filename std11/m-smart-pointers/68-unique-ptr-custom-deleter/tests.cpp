#include <gtest/gtest.h>

#include <cstdio>
#include <stdexcept>
#include <string>
#include <utility>

#include "solution.hpp"

namespace
{

std::string temp_path(const char *suffix)
{
    /* `::testing::TempDir()` honours the system tmpdir and is always
     * writable from a gtest binary. */
    return ::testing::TempDir() + "/task68_" + suffix + ".txt";
}

class file_handle_test : public ::testing::Test
{
protected:
    std::string path = temp_path(::testing::UnitTest::GetInstance()->current_test_info()->name());

    void TearDown() override
    {
        std::remove(path.c_str());
    }
};

}

TEST_F(file_handle_test, write_then_read_round_trip)
{
    {
        auto out = task68::open_write(path.c_str());
        ASSERT_NE(out.get(), nullptr);
        task68::write_line(out, "first");
        task68::write_line(out, "second");
        task68::write_line(out, "third");
    } /* fclose runs here */

    auto in = task68::open_read(path.c_str());
    ASSERT_NE(in.get(), nullptr);
    EXPECT_EQ(task68::read_all(in), "first\nsecond\nthird\n");
}

TEST_F(file_handle_test, open_read_missing_file_yields_empty_handle)
{
    auto in = task68::open_read("/this/path/should/not/exist/task68");
    EXPECT_EQ(in.get(), nullptr);
    EXPECT_FALSE(static_cast<bool>(in));
}

TEST_F(file_handle_test, move_transfers_ownership)
{
    auto a = task68::open_write(path.c_str());
    ASSERT_NE(a.get(), nullptr);
    FILE *raw = a.get();

    auto b = std::move(a);
    EXPECT_EQ(a.get(), nullptr);
    EXPECT_EQ(b.get(), raw);

    /* Writing through the moved-to handle still works. */
    task68::write_line(b, "moved");
}

TEST_F(file_handle_test, write_line_on_empty_handle_throws)
{
    task68::file_handle empty(nullptr, &std::fclose);
    EXPECT_THROW(task68::write_line(empty, "anything"), std::runtime_error);
}

TEST_F(file_handle_test, read_all_on_empty_handle_throws)
{
    task68::file_handle empty(nullptr, &std::fclose);
    EXPECT_THROW(task68::read_all(empty), std::runtime_error);
}

TEST_F(file_handle_test, write_empty_file_reads_back_empty)
{
    {
        auto out = task68::open_write(path.c_str());
        ASSERT_NE(out.get(), nullptr);
    } /* close without writing */

    auto in = task68::open_read(path.c_str());
    ASSERT_NE(in.get(), nullptr);
    EXPECT_EQ(task68::read_all(in), "");
}

TEST(file_handle_size, deleter_costs_one_pointer)
{
    /* A `unique_ptr<T, FuncPtrDeleter>` carries the function pointer
     * inline (no EBO for non-stateless deleters). Two pointers total
     * is the expected size. This isn't a correctness test of the
     * student code, just a sanity assert on the chosen idiom. */
    EXPECT_EQ(sizeof(task68::file_handle), 2 * sizeof(void *));
}
