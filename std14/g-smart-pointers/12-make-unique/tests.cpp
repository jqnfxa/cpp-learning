#include <gtest/gtest.h>

#include <cstddef>
#include <memory>
#include <string>

#include "solution.hpp"

namespace t = task12;

TEST(make_unique, widget_not_null)
{
    auto p = t::make_widget(42);
    ASSERT_NE(p, nullptr);
}

TEST(make_unique, widget_holds_value)
{
    auto p = t::make_widget(42);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->x, 42);
}

TEST(make_unique, widget_zero)
{
    auto p = t::make_widget(0);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->x, 0);
}

TEST(make_unique, widget_negative)
{
    auto p = t::make_widget(-123);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->x, -123);
}

TEST(make_unique, widget_unique)
{
    auto a = t::make_widget(1);
    auto b = t::make_widget(2);
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    /* Two unique_ptrs must not point to the same storage. */
    EXPECT_NE(a.get(), b.get());
}

TEST(make_unique, string_filled)
{
    auto p = t::make_string_n(5, 'x');
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(*p, "xxxxx");
}

TEST(make_unique, string_empty)
{
    auto p = t::make_string_n(0, 'q');
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(*p, "");
}

TEST(make_unique, string_large)
{
    auto p = t::make_string_n(100, '-');
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->size(), 100u);
    EXPECT_EQ((*p)[0], '-');
    EXPECT_EQ((*p)[99], '-');
}

TEST(make_unique, array_not_null)
{
    auto p = t::make_array(10);
    ASSERT_NE(p, nullptr);
}

TEST(make_unique, array_value_initialized)
{
    /* The array form of make_unique value-initialises every
     * element. For int[N] that means zero. */
    auto p = t::make_array(10);
    ASSERT_NE(p, nullptr);
    for (std::size_t i = 0; i < 10; ++i)
    {
        EXPECT_EQ(p[i], 0);
    }
}

TEST(make_unique, array_writable)
{
    auto p = t::make_array(5);
    ASSERT_NE(p, nullptr);
    for (std::size_t i = 0; i < 5; ++i)
    {
        p[i] = static_cast<int>(i * i);
    }
    EXPECT_EQ(p[0], 0);
    EXPECT_EQ(p[4], 16);
}

TEST(make_unique, ownership_transfer)
{
    /* make_unique gives ownership to the caller; moving the
     * unique_ptr leaves the source empty. */
    auto a = t::make_widget(7);
    ASSERT_NE(a, nullptr);
    auto b = std::move(a);
    EXPECT_EQ(a, nullptr);
    ASSERT_NE(b, nullptr);
    EXPECT_EQ(b->x, 7);
}
