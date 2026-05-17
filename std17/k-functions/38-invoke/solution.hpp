#pragma once

#include <functional>

namespace task38
{

struct widget
{
    int value;
    int get() const
    {
        return value;
    }
};

inline int free_double(int x)
{
    return x * 2;
}

/* TODO: implement each via std::invoke. */
inline int call_free(int x)
{
    (void)x;
    return 0;
}
inline int call_member(widget w)
{
    (void)w;
    return 0;
}
inline int call_lambda(int x)
{
    (void)x;
    return 0;
}

}
