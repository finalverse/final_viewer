// Copyright (c) 2026 Finalverse contributors. SPDX-License-Identifier: LGPL-2.1-only
#include "linden_common.h"
#include "../test/lltut.h"
#include "../lltoastordering.h"

namespace tut
{
struct toastordering_data
{
    struct Toast
    {
        double remaining;
        mutable int reads = 0;
        double getTimeLeftToLive() const { ++reads; return remaining - reads * .00001; }
    };
    struct Handle { Toast* toast; Toast* get() const { return toast; } };
};
typedef test_group<toastordering_data> toastordering_group;
typedef toastordering_group::object toastordering_object;
toastordering_group toastordering("lltoastordering");

template<> template<> void toastordering_object::test<1>()
{
    Toast a{10}; std::vector<Handle> handles{{&a}};
    ll_sort_toast_handles(handles);
    ensure_equals("sample a running clock only once", a.reads, 1);
}
template<> template<> void toastordering_object::test<2>()
{
    Toast a{10}, b{10}, c{20}; std::vector<Handle> handles{{&a}, {&b}, {&c}};
    ll_sort_toast_handles(handles);
    ensure("longest-lived first", handles[0].get() == &c);
    ensure("equal lifetimes remain stable", handles[1].get() == &a && handles[2].get() == &b);
    ensure_equals("one snapshot per timer", a.reads + b.reads + c.reads, 3);
}
template<> template<> void toastordering_object::test<3>()
{
    Toast invalid{std::numeric_limits<double>::quiet_NaN()}, active{10};
    std::vector<Handle> handles{{nullptr}, {&invalid}, {&active}};
    ll_sort_toast_handles(handles);
    ensure("valid toast precedes null/non-finite timers", handles[0].get() == &active);
    ensure("invalid entries remain stable", handles[1].get() == nullptr && handles[2].get() == &invalid);
}
}
