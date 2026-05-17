#include <gtest/gtest.h>

#include "solution.hpp"

TEST(nested_namespaces, answer_is_42)
{
    EXPECT_EQ(task15::deep::nest::answer(), 42);
}

TEST(nested_namespaces, label_is_deep_nest)
{
    EXPECT_EQ(task15::deep::nest::label(), "deep-nest");
}

TEST(nested_namespaces, via_alias)
{
    namespace dn = task15::deep::nest;
    EXPECT_EQ(dn::answer(), 42);
    EXPECT_EQ(dn::label(), "deep-nest");
}

TEST(nested_namespaces, names_callable_from_inner_scope)
{
    /* Add a using-declaration inside a function to import a
     * specific name. The C++17 nested form lets us write
     * the namespace path tersely. */
    using task15::deep::nest::answer;
    EXPECT_EQ(answer(), 42);
}
