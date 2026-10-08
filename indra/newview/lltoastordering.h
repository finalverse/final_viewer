// Copyright (c) 2026 Finalverse contributors. SPDX-License-Identifier: LGPL-2.1-only
#ifndef LL_LLTOASTORDERING_H
#define LL_LLTOASTORDERING_H

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>
#include <vector>

// Live timers change during sort, including when the comparator receives the
// same toast twice. Snapshot them so the ordering remains strict and stable.
template <class Handle>
void ll_sort_toast_handles(std::vector<Handle>& handles)
{
    std::vector<std::pair<double, Handle>> snapshot;
    snapshot.reserve(handles.size());
    for (const auto& handle : handles)
    {
        auto* toast = handle.get();
        double time = toast ? toast->getTimeLeftToLive() : -std::numeric_limits<double>::infinity();
        if (!std::isfinite(time)) time = -std::numeric_limits<double>::infinity();
        snapshot.emplace_back(time, handle);
    }
    std::stable_sort(snapshot.begin(), snapshot.end(), [](const auto& first, const auto& second) {
        return first.first > second.first;
    });
    for (size_t i = 0; i < handles.size(); ++i) handles[i] = snapshot[i].second;
}

#endif
