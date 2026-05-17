#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "solution.hpp"

namespace
{

/* Long enough to escape SSO on libstdc++ / libc++. */
std::vector<std::string> make_long_strings()
{
    return {
        std::string("alpha-long-enough-to-need-heap-storage-aaaaaaaaaaaaaaa"),
        std::string("beta--long-enough-to-need-heap-storage-bbbbbbbbbbbbbbb"),
        std::string("gamma-long-enough-to-need-heap-storage-ccccccccccccccc"),
        std::string("delta-long-enough-to-need-heap-storage-ddddddddddddddd"),
    };
}

}

TEST(move_algorithm, move_into_new_yields_correct_values)
{
    auto src_orig = make_long_strings();
    auto src = src_orig;
    auto dst = task83::move_into_new(src);
    EXPECT_EQ(dst, src_orig);
}

TEST(move_algorithm, move_into_new_empties_source_elements)
{
    auto src = make_long_strings();
    const std::size_t n = src.size();

    (void)task83::move_into_new(src);

    ASSERT_EQ(src.size(), n) << "source vector size is preserved";
    for (std::size_t i = 0; i < n; ++i)
    {
        EXPECT_TRUE(src[i].empty()) << "src[" << i << "] should be moved-from / empty";
    }
}

TEST(move_algorithm, move_into_new_steals_heap_buffers)
{
    auto src = make_long_strings();
    std::vector<const char *> captured;
    for (const auto &s : src)
    {
        captured.push_back(s.data());
    }

    auto dst = task83::move_into_new(src);
    ASSERT_EQ(dst.size(), captured.size());
    for (std::size_t i = 0; i < dst.size(); ++i)
    {
        EXPECT_EQ(dst[i].data(), captured[i]) << "heap-buffer transfer at i=" << i;
    }
}

TEST(move_algorithm, shift_right_by_one_basic)
{
    std::vector<std::string> v{"A", "B", "C", "D"};
    task83::shift_right_by_one(v);
    /* v[0] is moved-from (empty in practice for these short strings is
     * implementation-defined; for portability we test v[1..end] only
     * and rely on the slot still being assignable). */
    ASSERT_EQ(v.size(), 4u);
    EXPECT_EQ(v[1], "A");
    EXPECT_EQ(v[2], "B");
    EXPECT_EQ(v[3], "C");
    /* The original "D" is gone (overwritten by moved-in "C"). */
}

TEST(move_algorithm, shift_right_by_one_long_strings_preserves_buffers)
{
    auto v = make_long_strings();
    std::vector<const char *> captured;
    for (const auto &s : v)
    {
        captured.push_back(s.data());
    }

    task83::shift_right_by_one(v);

    /* After the shift:
     *   v[1] holds what was originally at v[0] (its heap buffer)
     *   v[2] holds what was originally at v[1]
     *   v[3] holds what was originally at v[2]
     *   the original v[3] buffer is freed when overwritten. */
    EXPECT_EQ(v[1].data(), captured[0]);
    EXPECT_EQ(v[2].data(), captured[1]);
    EXPECT_EQ(v[3].data(), captured[2]);
}

TEST(move_algorithm, shift_right_by_one_empty_is_noop)
{
    std::vector<std::string> v;
    task83::shift_right_by_one(v);
    EXPECT_TRUE(v.empty());
}

TEST(move_algorithm, shift_right_by_one_single_element_is_noop)
{
    std::vector<std::string> v{"only"};
    task83::shift_right_by_one(v);
    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], "only");
}

TEST(move_algorithm, shift_right_by_one_two_elements)
{
    std::vector<std::string> v{"A", "B"};
    task83::shift_right_by_one(v);
    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(v[1], "A");
    /* The original "B" is gone; v[0] is moved-from. */
}
