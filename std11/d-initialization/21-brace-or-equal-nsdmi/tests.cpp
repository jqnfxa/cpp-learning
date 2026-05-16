#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "solution.hpp"

TEST(nsdmi, default_constructed_retries)
{
    task21::config c{};
    EXPECT_EQ(c.retries, 3);
}

TEST(nsdmi, default_constructed_timeout)
{
    task21::config c{};
    EXPECT_EQ(c.timeout_ms, 5000);
}

TEST(nsdmi, default_constructed_name)
{
    task21::config c{};
    EXPECT_EQ(c.name, "default");
}

TEST(nsdmi, default_constructed_verbose)
{
    task21::config c{};
    EXPECT_FALSE(c.verbose);
}

TEST(nsdmi, default_constructed_tags_is_empty_vector)
{
    task21::config c{};
    EXPECT_TRUE(c.tags.empty());
}

TEST(nsdmi, fields_are_independently_mutable)
{
    task21::config a{};
    task21::config b{};

    a.retries = 7;
    a.name = "override";
    a.tags.push_back("alpha");

    /* a was changed... */
    EXPECT_EQ(a.retries, 7);
    EXPECT_EQ(a.name, "override");
    ASSERT_EQ(a.tags.size(), 1u);
    EXPECT_EQ(a.tags[0], "alpha");

    /* ...but b still has the NSDMI defaults. */
    EXPECT_EQ(b.retries, 3);
    EXPECT_EQ(b.name, "default");
    EXPECT_TRUE(b.tags.empty());
}

TEST(nsdmi, second_construction_uses_defaults_again)
{
    /*
     * A second default-constructed instance must read the NSDMIs,
     * not state from the first. This is the loose-equivalent of
     * "default values aren't captured at first instantiation".
     */
    {
        task21::config c{};
        c.retries = 999;
    }
    task21::config c2{};
    EXPECT_EQ(c2.retries, 3);
}
