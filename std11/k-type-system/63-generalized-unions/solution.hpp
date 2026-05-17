#pragma once

#include <new>
#include <string>

/*
 * Step 63 -- generalized (non-trivial) unions. See task.md for the spec.
 *
 * Complete int_or_string so it can hold either an int or a std::string,
 * with placement-new in each branch and a discriminator tag.
 */

namespace task63
{

class int_or_string
{
public:
    enum tag_t
    {
        is_int,
        is_string
    };

    /* TODO: construct from int; set tag_ = is_int and initialise int_. */
    int_or_string(int /*n*/)
        : tag_(is_int), int_(0) {}

    /* TODO: construct from std::string;
     *       set tag_ = is_string and `new (&string_) std::string(s);`. */
    int_or_string(const std::string & /*s*/)
        : tag_(is_int), int_(0) {}

    /* TODO: deep-copy the active alternative based on other.tag_. */
    int_or_string(const int_or_string & /*other*/)
        : tag_(is_int), int_(0) {}

    /* TODO: if tag_ == is_string, call string_.~basic_string(). */
    ~int_or_string() {}

    tag_t which() const noexcept
    {
        return tag_;
    }

    int as_int() const noexcept
    {
        return int_;
    }

    /* TODO: return string_ when alive. */
    const std::string &as_string() const noexcept
    {
        static const std::string empty;
        return empty;
    }

    /* TODO: if currently a string, destroy it; then store n; set tag_. */
    void assign(int /*n*/) {}

    /* TODO: if currently a string, assign normally;
     *       else placement-new a string into the slot and set tag_. */
    void assign(const std::string & /*s*/) {}

private:
    tag_t tag_;
    union
    {
        int int_;
        std::string string_;
    };
};

}
