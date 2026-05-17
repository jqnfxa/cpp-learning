#include <gtest/gtest.h>

#include <functional>
#include <stdexcept>

#include "solution.hpp"

namespace
{
const auto kTable = task94::make_arithmetic_table();
}

TEST(std_function, negate)
{
    EXPECT_EQ(task94::apply(kTable, "negate", 5), -5);
    EXPECT_EQ(task94::apply(kTable, "negate", -3), 3);
    EXPECT_EQ(task94::apply(kTable, "negate", 0), 0);
}

TEST(std_function, double_op)
{
    EXPECT_EQ(task94::apply(kTable, "double", 7), 14);
    EXPECT_EQ(task94::apply(kTable, "double", -1), -2);
    EXPECT_EQ(task94::apply(kTable, "double", 0), 0);
}

TEST(std_function, square)
{
    EXPECT_EQ(task94::apply(kTable, "square", 4), 16);
    EXPECT_EQ(task94::apply(kTable, "square", -3), 9);
    EXPECT_EQ(task94::apply(kTable, "square", 0), 0);
}

TEST(std_function, succ)
{
    EXPECT_EQ(task94::apply(kTable, "succ", 0), 1);
    EXPECT_EQ(task94::apply(kTable, "succ", -1), 0);
    EXPECT_EQ(task94::apply(kTable, "succ", 42), 43);
}

TEST(std_function, pred)
{
    EXPECT_EQ(task94::apply(kTable, "pred", 1), 0);
    EXPECT_EQ(task94::apply(kTable, "pred", 0), -1);
    EXPECT_EQ(task94::apply(kTable, "pred", 42), 41);
}

TEST(std_function, missing_key_throws_out_of_range)
{
    EXPECT_THROW(task94::apply(kTable, "nonexistent", 1), std::out_of_range);
    EXPECT_THROW(task94::apply(kTable, "", 1), std::out_of_range);
}

TEST(std_function, table_has_five_entries)
{
    EXPECT_EQ(kTable.size(), 5u);
    EXPECT_EQ(kTable.count("negate"), 1u);
    EXPECT_EQ(kTable.count("double"), 1u);
    EXPECT_EQ(kTable.count("square"), 1u);
    EXPECT_EQ(kTable.count("succ"), 1u);
    EXPECT_EQ(kTable.count("pred"), 1u);
}

TEST(std_function, table_entries_are_callable)
{
    /* Direct verification of the std::function slots -- they are real
     * std::functions, callable independently from `apply`. */
    auto t = task94::make_arithmetic_table();
    EXPECT_TRUE(static_cast<bool>(t.at("negate")));
    EXPECT_EQ(t.at("negate")(10), -10);
    EXPECT_EQ(t.at("square")(6), 36);
}

TEST(std_function, can_add_new_entries_at_runtime)
{
    auto t = task94::make_arithmetic_table();

    /* Demonstrate the "uniform std::function type" point of the
     * exercise: we can stuff a capture lambda into the same slot. */
    int k = 100;
    t["plus_k"] = [k](int x)
    { return x + k; };

    EXPECT_EQ(task94::apply(t, "plus_k", 5), 105);
    EXPECT_EQ(t.size(), 6u);
}
