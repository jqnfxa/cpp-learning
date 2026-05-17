#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "solution.hpp"

namespace
{

/* Long enough to escape any reasonable SSO buffer
 * (libstdc++ ~15, libc++ ~22). */
const char *kLongString = "this is a string that is too long for the SSO buffer of any STL";

}

TEST(move_into_container, push_copy_leaves_source_intact)
{
    std::vector<std::string> v;
    std::string s(kLongString);

    task78::push_copy(v, s);

    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], kLongString);
    EXPECT_EQ(s, kLongString) << "push_copy must not mutate the source";
    EXPECT_EQ(s.size(), std::string(kLongString).size());
}

TEST(move_into_container, push_copy_allocates_new_buffer)
{
    std::vector<std::string> v;
    std::string s(kLongString);
    const char *before = s.data();

    task78::push_copy(v, s);

    ASSERT_EQ(v.size(), 1u);
    EXPECT_NE(v[0].data(), before)
        << "copy should allocate a new buffer, not steal s's";
    EXPECT_EQ(s.data(), before) << "s still owns its original buffer";
}

TEST(move_into_container, push_move_empties_source)
{
    std::vector<std::string> v;
    std::string s(kLongString);

    task78::push_move(v, s);

    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], kLongString);
    /* For a heap-allocated source, both libstdc++ and libc++ empty
     * the moved-from string. This is implementation-defined in
     * general; we rely on the major-STL behaviour for the test. */
    EXPECT_TRUE(s.empty()) << "after move, source string is empty (libstdc++/libc++)";
    EXPECT_EQ(s.size(), 0u);
}

TEST(move_into_container, push_move_steals_heap_buffer)
{
    std::vector<std::string> v;
    std::string s(kLongString);
    const char *before = s.data();

    task78::push_move(v, s);

    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0].data(), before)
        << "move should transfer the heap buffer to v[0]";
}

TEST(move_into_container, push_move_loop_does_not_grow_allocations)
{
    /* Repeatedly push the same captured string via move. Each
     * iteration re-creates s, captures its data pointer, moves it
     * into v, and asserts v.back() points at the captured buffer.
     * If the implementation accidentally copies, the captured
     * pointers wouldn't match the vector entries. */
    std::vector<std::string> v;
    v.reserve(8);
    std::vector<const char *> captured;

    for (int i = 0; i < 8; ++i)
    {
        std::string s = std::string(kLongString) + std::to_string(i);
        captured.push_back(s.data());
        task78::push_move(v, s);
    }

    ASSERT_EQ(v.size(), 8u);
    for (std::size_t i = 0; i < 8; ++i)
    {
        EXPECT_EQ(v[i].data(), captured[i]) << "iteration " << i;
    }
}
