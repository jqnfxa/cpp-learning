#pragma once

#include <memory>

/*
 * Step 23 -- replace auto_ptr with unique_ptr (C++17 removal).
 *
 * `std::auto_ptr` was removed in C++17. Use `std::unique_ptr`
 * which has proper move semantics.
 */

namespace task23
{

struct widget
{
    int value;
    explicit widget(int v)
        : value(v)
    {
    }
};

/* TODO: rewrite to use std::unique_ptr<widget>:
 *
 *   inline std::unique_ptr<widget> make_widget(int v) {
 *       return std::make_unique<widget>(v);
 *   }
 */
/* TODO: return std::make_unique<widget>(v); */
inline std::unique_ptr<widget> make_widget(int v)
{
    (void)v;
    return nullptr;
}

/* TODO:
 *   auto local = std::move(p);
 *   return local->value;
 */
inline int move_and_read(std::unique_ptr<widget> p)
{
    (void)p;
    return 0;
}

}
