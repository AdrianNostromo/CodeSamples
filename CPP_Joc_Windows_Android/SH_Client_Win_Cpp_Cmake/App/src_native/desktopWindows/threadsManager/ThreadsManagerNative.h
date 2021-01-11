#pragma once

#include <base/gh.h>
#include <base/threadsManager/ThreadsManager.h>

namespace desktopWindows {
class ThreadsManagerNative : public base::ThreadsManager {priv typedef base::ThreadsManager super;pub dCtor(ThreadsManagerNative);
    pub explicit ThreadsManagerNative();

    prot base::ManagedThread* getNewManagedThreadClass(std::string& name, std::function<unsigned int(base::IManagedThread* managedThread)> cbThreadTick) final;

    pub ~ThreadsManagerNative() override;
};
};
