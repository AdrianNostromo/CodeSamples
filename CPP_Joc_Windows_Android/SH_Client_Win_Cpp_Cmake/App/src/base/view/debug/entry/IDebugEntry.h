#pragma once

#include <base/gh.h>
#include <string>
#include <memory>

namespace base {
    class IWrappedValue;
};

namespace base {
class IDebugEntry {
    pub virtual void invalidateData() = 0;
    pub virtual void remove() = 0;

    pub virtual void setLinesCount(int linesCount) = 0;
    pub virtual void setToggleSwitchIsOn(bool isOn) = 0;

    pub virtual ~IDebugEntry() = default;
};
};
