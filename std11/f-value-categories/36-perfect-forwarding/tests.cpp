#include <gtest/gtest.h>

#include <string>
#include <utility>

#include "solution.hpp"

/*
 * `sink` is the test type: each constructor records a distinct
 * sentinel in `from`, so we can tell which one was picked.
 */
namespace
{

struct sink
{
    int from;

    sink()
        : from(0)
    {
    }

    sink(const std::string & /* s */)
        : from(1)
    {
    }

    sink(std::string && /* s */)
        : from(2)
    {
    }

    sink(int /* n */)
        : from(3)
    {
    }

    sink(int /* n */, const std::string & /* s */)
        : from(4)
    {
    }

    sink(int /* n */, std::string && /* s */)
        : from(5)
    {
    }
};

}

TEST(perfect_forwarding, no_args_calls_default_ctor)
{
    sink s = task36::factory<sink>();
    EXPECT_EQ(s.from, 0);
}

TEST(perfect_forwarding, lvalue_string_calls_const_ref_ctor)
{
    std::string str = "hello";
    sink s = task36::factory<sink>(str);
    EXPECT_EQ(s.from, 1);
}

TEST(perfect_forwarding, rvalue_string_calls_rvalue_ref_ctor)
{
    sink s = task36::factory<sink>(std::string{"hello"});
    EXPECT_EQ(s.from, 2);
}

TEST(perfect_forwarding, std_move_lvalue_calls_rvalue_ref_ctor)
{
    std::string str = "hello";
    sink s = task36::factory<sink>(std::move(str));
    EXPECT_EQ(s.from, 2);
}

TEST(perfect_forwarding, int_calls_int_ctor)
{
    sink s = task36::factory<sink>(42);
    EXPECT_EQ(s.from, 3);
}

TEST(perfect_forwarding, int_plus_lvalue_string_calls_const_ref_ctor)
{
    std::string str = "hello";
    sink s = task36::factory<sink>(42, str);
    EXPECT_EQ(s.from, 4);
}

TEST(perfect_forwarding, int_plus_rvalue_string_calls_rvalue_ref_ctor)
{
    sink s = task36::factory<sink>(42, std::string{"hello"});
    EXPECT_EQ(s.from, 5);
}
