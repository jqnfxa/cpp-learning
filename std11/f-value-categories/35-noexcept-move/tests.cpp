#include <gtest/gtest.h>

#include <type_traits>
#include <utility>

#include "solution.hpp"

/* The headline assertion: noexcept-ness of the move ctor flows
 * directly into is_nothrow_move_constructible. */
static_assert(std::is_nothrow_move_constructible<task35::safe_mover>::value, "safe_mover's move ctor must be noexcept");
static_assert(!std::is_nothrow_move_constructible<task35::unsafe_mover>::value, "unsafe_mover's move ctor must NOT be noexcept");

/* Both types must still be move-constructible (just one is "throwing"). */
static_assert(std::is_move_constructible<task35::safe_mover>::value, "safe_mover is move-constructible");
static_assert(std::is_move_constructible<task35::unsafe_mover>::value, "unsafe_mover is move-constructible");

/* std::move_if_noexcept returns T&& for noexcept move types, const T& otherwise. */
static_assert(std::is_same<decltype(std::move_if_noexcept(std::declval<task35::safe_mover &>())), task35::safe_mover &&>::value, "move_if_noexcept yields T && for safe_mover");
static_assert(std::is_same<decltype(std::move_if_noexcept(std::declval<task35::unsafe_mover &>())), const task35::unsafe_mover &>::value, "move_if_noexcept yields const T & for unsafe_mover");

TEST(noexcept_move, safe_mover_move_ctor_is_noexcept)
{
    EXPECT_TRUE(std::is_nothrow_move_constructible<task35::safe_mover>::value);
}

TEST(noexcept_move, unsafe_mover_move_ctor_is_not_noexcept)
{
    EXPECT_FALSE(std::is_nothrow_move_constructible<task35::unsafe_mover>::value);
}

TEST(noexcept_move, noexcept_operator_directly)
{
    /* The noexcept operator inspects an expression for the
     * possibility of throwing. */
    task35::safe_mover s;
    task35::unsafe_mover u;
    EXPECT_TRUE(noexcept(task35::safe_mover{std::move(s)}));
    EXPECT_FALSE(noexcept(task35::unsafe_mover{std::move(u)}));
}

TEST(noexcept_move, both_types_default_constructible)
{
    task35::safe_mover s;
    task35::unsafe_mover u;
    (void)s;
    (void)u;
    SUCCEED();
}
