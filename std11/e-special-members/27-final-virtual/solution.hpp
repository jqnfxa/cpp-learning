#pragma once

/*
 * Step 27 -- final on virtual functions. See task.md.
 *
 * Implement a 3-level hierarchy where middle's override of rate()
 * is marked `final`, preventing further overrides. Provide bodies
 * for rate() and kind() with the return values from task.md.
 */

namespace task27
{

class base
{
public:
    virtual ~base() = default;

    /* TODO: virtual, return 1. */
    virtual int rate() const
    {
        return 0;
    }
};

class middle : public base
{
public:
    /* TODO: override final, return 2. */
    int rate() const override
    {
        return 0;
    }
};

class leaf : public middle
{
public:
    /* TODO: return 3. NOT marked override -- it's a new function,
     * not a replacement for rate(). */
    int kind() const
    {
        return 0;
    }
};

}
