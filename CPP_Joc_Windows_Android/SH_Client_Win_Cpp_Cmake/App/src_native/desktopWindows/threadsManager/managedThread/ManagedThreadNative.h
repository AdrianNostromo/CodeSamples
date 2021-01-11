#pragma once

#include <base/gh.h>
#include <base/threadsManager/managedThread/ManagedThread.h>
#include <thread>

namespace desktopWindows {
class ManagedThreadNative : public base::ManagedThread {priv typedef base::ManagedThread super;pub dCtor(ManagedThreadNative);
    pub std::thread* t = nullptr;

    pub explicit ManagedThreadNative(
        base::IHandler_ManagedThread* handler,
        std::string& name,
        std::function<unsigned int(base::IManagedThread* managedThread)> cbThreadTick);
    
    pub void startNativeThread() final;

    // Return 0: no sleep;
    // Return >0: sleep duration;
    // Return <0: exit thread loop;
    pub int onThreadTick() override;

    pub void onNativeThreadExited() override;

    prot void disposeMain() override;
    pub ~ManagedThreadNative() override;
};
};
