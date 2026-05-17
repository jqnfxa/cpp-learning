#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

/*
 * Step 71 -- std::weak_ptr<T>. See task.md for the spec.
 *
 * The provided code has a parent/child cycle: Child::parent is a
 * shared_ptr<Parent>, which Parent's shared_ptr<Child> mirrors.
 * Change the back-reference to a weak_ptr to break the cycle.
 */

namespace task71
{

struct Parent;
struct Child;

/* Lifetime instrumentation -- counters incremented in ctor,
 * decremented in dtor. Tests inspect these to detect leaks. */
inline int &parents_alive()
{
    static int n = 0;
    return n;
}

inline int &children_alive()
{
    static int n = 0;
    return n;
}

struct Parent
{
    std::string name;
    std::vector<std::shared_ptr<Child>> children;

    explicit Parent(std::string n)
        : name(std::move(n))
    {
        ++parents_alive();
    }

    ~Parent()
    {
        --parents_alive();
    }
};

struct Child
{
    std::string name;

    /* TODO: change this to `std::weak_ptr<Parent> parent;` so the
     *       Parent -> Child -> Parent cycle is broken. */
    std::shared_ptr<Parent> parent;

    explicit Child(std::string n)
        : name(std::move(n))
    {
        ++children_alive();
    }

    ~Child()
    {
        --children_alive();
    }

    /* TODO: lock() the weak_ptr; return its name if alive, "" otherwise.
     *
     *   if (auto sp = parent.lock()) return sp->name;
     *   return {};
     */
    std::string parent_name() const
    {
        if (parent)
        {
            return parent->name;
        }
        return {};
    }
};

inline std::shared_ptr<Parent> make_parent(std::string name)
{
    return std::make_shared<Parent>(std::move(name));
}

inline std::shared_ptr<Child> make_child(std::string name)
{
    return std::make_shared<Child>(std::move(name));
}

/* Add `c` to `p`'s children vector and set `c->parent = p`. This
 * function works regardless of whether `c->parent` is `shared_ptr` or
 * `weak_ptr` -- the assignment is the same. */
inline void link(const std::shared_ptr<Parent> &p, const std::shared_ptr<Child> &c)
{
    p->children.push_back(c);
    c->parent = p;
}

}
