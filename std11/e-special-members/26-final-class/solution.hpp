#pragma once

/*
 * Step 26 -- final on classes. See task.md for the spec.
 *
 * Declare two classes inside task26: `sealed` (with the `final`
 * specifier) and `openable` (without). Both must have an accessor
 * `int value() const` returning the NSDMI default below.
 */

namespace task26
{

/* TODO: add `final` between the class name and the opening brace. */
class sealed
{
public:
    int value() const
    {
        return value_;
    }

private:
    int value_ = 42;
};

class openable
{
public:
    int value() const
    {
        return value_;
    }

private:
    int value_ = 7;
};

}
