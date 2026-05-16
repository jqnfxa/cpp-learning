#include <gtest/gtest.h>

#include <string>

#include "solution.hpp"

TEST(override_keyword, base_returns_1)
{
    task28::base b{};
    EXPECT_EQ(b.process("anything"), 1);
}

TEST(override_keyword, derived_returns_2)
{
    task28::derived d{};
    EXPECT_EQ(d.process("anything"), 2);
}

TEST(override_keyword, virtual_dispatch_through_base_pointer)
{
    task28::derived d{};
    task28::base *p = &d;
    /* If derived::process wasn't a real override (e.g. by-value
     * parameter), this call would dispatch to base::process and
     * return 1. The fact that it returns 2 proves the signature
     * matches. */
    EXPECT_EQ(p->process("anything"), 2);
}

TEST(override_keyword, accepts_temporary_string)
{
    /* Only callable if the parameter is `const std::string &` (or by
     * value). If the student wrote `std::string &input` (non-const
     * reference), this call wouldn't bind. */
    task28::derived d{};
    EXPECT_EQ(d.process(std::string{"temporary"}), 2);
}

TEST(override_keyword, accepts_c_string_literal)
{
    /* C-string literal converts to std::string for the const-ref
     * parameter. */
    task28::derived d{};
    EXPECT_EQ(d.process("c-literal"), 2);
}
