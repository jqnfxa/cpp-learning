#pragma once

#include <string>
#include <utility>

/*
 * Step 23 -- inheriting constructors with `using Base::Base`.
 * See task.md for the spec.
 *
 * `base` is provided. You declare `derived` and inherit every
 * constructor from `base` with one `using` declaration.
 */

namespace task23
{

class base
{
public:
    base()
        : value_(0), name_("default")
    {
    }

    base(int v)
        : value_(v), name_("default")
    {
    }

    base(int v, const std::string &n)
        : value_(v), name_(n)
    {
    }

    int value() const
    {
        return value_;
    }

    const std::string &name() const
    {
        return name_;
    }

private:
    int value_;
    std::string name_;
};

class derived : public base
{
public:
    /* TODO: inherit every base constructor with one `using` declaration. */
};

}
