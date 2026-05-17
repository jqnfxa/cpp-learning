#pragma once

#include <memory>
#include <vector>

/*
 * Step 72 -- std::enable_shared_from_this. See task.md for the spec.
 *
 * Make Widget inherit from std::enable_shared_from_this<Widget>, and
 * implement self() and subscribe() via shared_from_this().
 */

namespace task72
{

inline int &widgets_alive()
{
    static int n = 0;
    return n;
}

/* TODO: change to `class Widget : public std::enable_shared_from_this<Widget>` */
class Widget
{
    int value_;

public:
    explicit Widget(int v)
        : value_(v)
    {
        ++widgets_alive();
    }

    ~Widget()
    {
        --widgets_alive();
    }

    int value() const
    {
        return value_;
    }

    /* TODO: return shared_from_this(). */
    std::shared_ptr<Widget> self()
    {
        return std::shared_ptr<Widget>();
    }

    /* TODO: roster.push_back(shared_from_this()). */
    void subscribe(std::vector<std::shared_ptr<Widget>> & /*roster*/)
    {
    }
};

inline std::shared_ptr<Widget> make_widget(int v)
{
    return std::make_shared<Widget>(v);
}

}
