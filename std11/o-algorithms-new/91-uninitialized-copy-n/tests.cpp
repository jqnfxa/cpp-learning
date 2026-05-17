#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "solution.hpp"

namespace
{

std::vector<std::string> make_long_strings()
{
    /* Each one heap-allocated (well past SSO). Tests verify that the
     * copies have correct content -- the heap data was copy-constructed,
     * not aliased. */
    return {
        std::string("alpha-long-enough-to-need-heap-storage-aaaaaaaaaaaaaaa"),
        std::string("beta--long-enough-to-need-heap-storage-bbbbbbbbbbbbbbb"),
        std::string("gamma-long-enough-to-need-heap-storage-ccccccccccccccc"),
        std::string("delta-long-enough-to-need-heap-storage-ddddddddddddddd"),
        std::string("eps---long-enough-to-need-heap-storage-eeeeeeeeeeeeeee"),
    };
}

}

TEST(uninitialized_copy_n, copy_prefix)
{
    auto src = make_long_strings();
    auto out = task91::via_uninitialized_copy_n(src, 3);

    ASSERT_EQ(out.size(), 3u);
    EXPECT_EQ(out[0], src[0]);
    EXPECT_EQ(out[1], src[1]);
    EXPECT_EQ(out[2], src[2]);
}

TEST(uninitialized_copy_n, copy_all)
{
    auto src = make_long_strings();
    auto out = task91::via_uninitialized_copy_n(src, src.size());
    EXPECT_EQ(out, src);
}

TEST(uninitialized_copy_n, copy_zero)
{
    auto src = make_long_strings();
    auto out = task91::via_uninitialized_copy_n(src, 0);
    EXPECT_TRUE(out.empty());
}

TEST(uninitialized_copy_n, source_not_aliased)
{
    /* The output vector must own its own buffers, not point at the
     * raw storage that was freed inside the function. We can verify
     * by mutating src after the call: out is untouched. */
    auto src = make_long_strings();
    auto out = task91::via_uninitialized_copy_n(src, 3);
    ASSERT_EQ(out.size(), 3u); /* guard before indexing */

    /* Mutate src; out must not change. */
    for (auto &s : src)
    {
        s = "mutated";
    }
    EXPECT_EQ(out[0].substr(0, 5), "alpha");
    EXPECT_EQ(out[1].substr(0, 4), "beta");
    EXPECT_EQ(out[2].substr(0, 5), "gamma");
}

TEST(uninitialized_copy_n, large_count_no_leak)
{
    /* If destructors aren't called on the raw-buffer elements after the
     * vector copy, every iteration leaks count strings. Under
     * ASAN/LSAN this fails loudly; without, it'll fail by exhausting
     * memory on big loops. We use a moderate size and rely on the
     * sanitiser to flag a real leak. */
    auto src = make_long_strings();
    for (int i = 0; i < 1024; ++i)
    {
        auto out = task91::via_uninitialized_copy_n(src, src.size());
        (void)out;
    }
    SUCCEED();
}

TEST(uninitialized_copy_n, output_independent_of_input)
{
    /* After the function returns, the raw buffer is gone and out
     * holds its own copies. Modifying out must not touch src. */
    auto src = make_long_strings();
    auto out = task91::via_uninitialized_copy_n(src, src.size());
    ASSERT_EQ(out.size(), src.size()); /* guard before indexing */

    out[0] = "modified";
    EXPECT_EQ(src[0].substr(0, 5), "alpha");
}
