#include <gtest/gtest.h>

#include "solution.hpp"

TEST(removed, passthrough_is_identity)
{
    EXPECT_EQ(task27::passthrough(42), 42);
}

TEST(removed, register_keyword_unused)
{
    /* Pre-C++17, `register int x = 5;` was a storage-class
     * hint. C++17 removed the storage-class meaning.
     * The keyword itself is RESERVED (so you can't use it
     * as an identifier). We can't directly test this since
     * `register` won't compile -- the lack of a code block
     * for it IS the test. */
    SUCCEED();
}
