#pragma once

#include <base/gh.h>

template <class _ArgType, class _ResultType>
class ICustomComparator1 {
    pub using ArgType = _ArgType;
    pub using ResultType = _ResultType;

    pub virtual ResultType operator()(ArgType o) const = 0;

    pub virtual ~ICustomComparator1() = default;
};
