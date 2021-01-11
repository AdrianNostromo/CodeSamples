#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>
#include "TM_Base.h"
#include "managedThread/ManagedThread.h"
#include "managedThread/IHandler_ManagedThread.h"
#include <base/MM.h>

namespace base {
class TM_ThreadsList : public TM_Base, virtual public IHandler_ManagedThread {priv typedef TM_Base super;pub dCtor(TM_ThreadsList);
    priv static const bool IS_DEBUG_LOCAL;

    priv NativeThreadsStatus nativeThreadsStatus = Off;
    priv bool isNatveThreadsPrefferedEnabled = false;

    priv ListDL<ManagedThread*> managedThreadsList{LOC};
    priv bool isAnyRemoveThreadsExited = false;

    priv int statistics_managedThreadsCount = 0;
    priv int statistics_nativeThreadsCount = 0;

    pub explicit TM_ThreadsList();

    pub void manageThreadsLoop() final;

    pub void rt_onManagedThreadExited(ManagedThread* managedThread) final;

    pub void rt_onNativeThreadStarted(ManagedThread* managedThread) final;
    pub void rt_onNativeThreadExited(ManagedThread* managedThread) final;

    pub NativeThreadsStatus getNativeThreadsStatus() final;
    pub void setIsNatveThreadsPrefferedEnabled(bool isNatveThreadsPrefferedEnabled) final;

    priv void setNativeThreadsStatus(NativeThreadsStatus nativeThreadsStatus);

    pub IManagedThread* newThread(std::string name, std::function<unsigned int(IManagedThread* managedThread)> cbThreadTick) final;
    
    prot virtual ManagedThread* getNewManagedThreadClass(std::string& name, std::function<unsigned int(IManagedThread* managedThread)> cbThreadTick) = 0;

    pub int getStatistics_managedThreadsCount() final;
    pub int getStatistics_nativeThreadsCount() final;

    prot void disposeMain() override;
    pub ~TM_ThreadsList() override;
};
};
