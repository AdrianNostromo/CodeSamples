#pragma once

#include <base/gh.h>
#include "IManagedThread.h"
#include <functional>
#include <base/object/Object.h>
#include <string>

namespace base {
    class IHandler_ManagedThread;
};

namespace base {
class ManagedThread : public Object, virtual public IManagedThread {priv typedef Object super;pub dCtor(ManagedThread);
    priv static const bool IS_DEBUG_LOCAL;

    prot IHandler_ManagedThread* handler;
    
    pub std::string name;

    priv std::function<unsigned int(base::IManagedThread* managedThread)> cbThreadTick;

    // This is called to stop and dispose the thread.
    pub bool isRemoveRequested = false;
    // This is used to stop the native thread only.
    pub bool isNativeThreadExitRequested = false;

    pub bool isThreadExited = false;

    pub explicit ManagedThread(
        IHandler_ManagedThread* handler,
        std::string& name,
        std::function<unsigned int(base::IManagedThread* managedThread)> cbThreadTick);

    pub void removeManaged() final;
    pub virtual void startNativeThread() = 0;
    pub virtual int onThreadTick();

    prot virtual void onNativeThreadStarted();
    prot virtual void onNativeThreadExited();

    pub virtual ~ManagedThread();
};
};
