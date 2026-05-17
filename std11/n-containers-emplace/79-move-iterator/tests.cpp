#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "solution.hpp"

namespace
{

/* Each element is long enough to escape SSO on all major STLs. */
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

TEST(move_iterator, copy_range_preserves_source)
{
    auto src = make_long_strings();
    auto src_snapshot = src;

    auto dst = task79::via_copy_range(src);

    EXPECT_EQ(dst, src_snapshot);
    EXPECT_EQ(src, src_snapshot) << "via_copy_range must not modify src";
}

TEST(move_iterator, copy_range_allocates_new_buffers)
{
    auto src = make_long_strings();
    std::vector<const char *> src_data;
    for (const auto &s : src)
    {
        src_data.push_back(s.data());
    }

    auto dst = task79::via_copy_range(src);
    ASSERT_EQ(dst.size(), src.size());
    for (std::size_t i = 0; i < dst.size(); ++i)
    {
        EXPECT_NE(dst[i].data(), src_data[i]) << "copy must allocate fresh buffers, i=" << i;
    }
}

TEST(move_iterator, move_range_yields_correct_values)
{
    auto src_orig = make_long_strings();
    auto src = src_orig;
    auto dst = task79::via_move_range(src);
    EXPECT_EQ(dst, src_orig);
}

TEST(move_iterator, move_range_steals_heap_buffers)
{
    auto src = make_long_strings();
    std::vector<const char *> src_data;
    for (const auto &s : src)
    {
        src_data.push_back(s.data());
    }

    auto dst = task79::via_move_range(src);
    ASSERT_EQ(dst.size(), src_data.size());
    for (std::size_t i = 0; i < dst.size(); ++i)
    {
        EXPECT_EQ(dst[i].data(), src_data[i]) << "move should transfer heap buffer, i=" << i;
    }
}

TEST(move_iterator, move_range_empties_source_elements)
{
    auto src = make_long_strings();
    const std::size_t n = src.size();

    auto dst = task79::via_move_range(src);

    /* For heap-allocated strings, both libstdc++ and libc++ empty the
     * source on move. The standard only promises "valid but
     * unspecified" -- we rely on the major-STL behaviour. */
    ASSERT_EQ(src.size(), n) << "size of the vector itself is preserved";
    for (std::size_t i = 0; i < n; ++i)
    {
        EXPECT_TRUE(src[i].empty()) << "i=" << i;
    }
}

TEST(move_iterator, move_range_size_matches)
{
    auto src = make_long_strings();
    auto dst = task79::via_move_range(src);
    EXPECT_EQ(dst.size(), 4u);
}
