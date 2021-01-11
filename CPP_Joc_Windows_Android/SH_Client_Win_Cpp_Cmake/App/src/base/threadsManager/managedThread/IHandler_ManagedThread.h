#pragma once

#include <base/gh.h>

namespace base {
    class ManagedThread;
};

namespace base {
class IHandler_ManagedThread {
    // rt = Remove thread
    pub virtual void rt_onManagedThreadExited(ManagedThread* managedThread) = 0;

    // rt = Remove thread
    pub virtual void rt_onNativeThreadStarted(ManagedThread* managedThread) = 0;
    // rt = Remove thread
    pub virtual void rt_onNativeThreadExited(ManagedThread* managedThread) = 0;

    pub virtual ~IHandler_ManagedThread() = default;
};
};
