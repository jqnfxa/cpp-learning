#pragma once

#include <string>

/*
 * Step 28 -- override. See task.md for the spec.
 *
 * base is provided. Declare derived (publicly inheriting from base)
 * with an override of process() that returns 2. The override must
 * be marked with the `override` keyword.
 */

namespace task28
{

class base
{
public:
    virtual ~base() = default;

    virtual int process(const std::string & /* input */) const
    {
        return 1;
    }
};

/* TODO: declare derived : public base with
 *   int process(const std::string &input) const override
 * returning 2. */
class derived : public base
{
};

}
