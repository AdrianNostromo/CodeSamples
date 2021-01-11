#pragma once

#include <base/gh.h>

namespace base {
    class DebugEntry;
    class IDebugEntry;
};

namespace base {
class IHandler_DebugEntry {
    pub virtual void onDebugEntryInvalidation(DebugEntry* entry) = 0;
    pub virtual void onDebugEntryLinesCountChanged(DebugEntry* entry) = 0;

    pub virtual void removeDebugEntry(DebugEntry* debugEntry) = 0;

    pub virtual ~IHandler_DebugEntry() = default;
};
};
