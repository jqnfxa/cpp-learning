#include <gtest/gtest.h>

#include <type_traits>

#include "solution.hpp"

/* The class must be trivially destructible to qualify as a literal type. */
static_assert(std::is_trivially_destructible<task40::point>::value, "point must have a trivial destructor");

/* Construct `point` in constant-expression contexts. */
constexpr task40::point origin{0, 0};
constexpr task40::point three_four{3, 4};

static_assert(origin.x() == 0, "origin.x() == 0");
static_assert(origin.y() == 0, "origin.y() == 0");
static_assert(three_four.x() == 3, "three_four.x() == 3");
static_assert(three_four.y() == 4, "three_four.y() == 4");

/* distance_squared must be constexpr too -- usable in static_assert. */
static_assert(task40::distance_squared(task40::point{0, 0}, task40::point{3, 4}) == 25, "3-4-5 triangle");
static_assert(task40::distance_squared(task40::point{1, 1}, task40::point{1, 1}) == 0, "same point");
static_assert(task40::distance_squared(task40::point{-1, -1}, task40::point{1, 1}) == 8, "diagonal");

TEST(literal_types, runtime_point_construction)
{
    task40::point p{5, 7};
    EXPECT_EQ(p.x(), 5);
    EXPECT_EQ(p.y(), 7);
}

TEST(literal_types, runtime_distance_squared)
{
    EXPECT_EQ(task40::distance_squared({0, 0}, {3, 4}), 25);
    EXPECT_EQ(task40::distance_squared({2, 2}, {5, 6}), 25);
    EXPECT_EQ(task40::distance_squared({10, 10}, {10, 10}), 0);
}

TEST(literal_types, distance_squared_is_symmetric)
{
    task40::point a{1, 2};
    task40::point b{4, 6};
    EXPECT_EQ(task40::distance_squared(a, b), task40::distance_squared(b, a));
}

TEST(literal_types, negative_coordinates)
{
    EXPECT_EQ(task40::distance_squared({-3, -4}, {0, 0}), 25);
    EXPECT_EQ(task40::distance_squared({-1, -1}, {1, 1}), 8);
}
