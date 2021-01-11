#pragma once

#include <base/gh.h>
#include <base/object/IManagedObject.h>
#include <functional>
#include <string>

namespace base {
    class IManagedThread;

class IThreadsManager : virtual public base::IManagedObject {
    pub enum NativeThreadsStatus { Off, Stopping, Active };
    
    pub virtual void manageThreadsLoop() = 0;

    pub virtual NativeThreadsStatus getNativeThreadsStatus() = 0;
    pub virtual void setIsNatveThreadsPrefferedEnabled(bool isNatveThreadsPrefferedEnabled) = 0;

    // This has an actual directly dedicated underlying thread.
    pub virtual IManagedThread* newThread(std::string name, std::function<unsigned int(IManagedThread* managedThread)> cbThreadTick) = 0;

    pub virtual int getStatistics_managedThreadsCount() = 0;
    pub virtual int getStatistics_nativeThreadsCount() = 0;

    pub ~IThreadsManager() override = default;
};
};
