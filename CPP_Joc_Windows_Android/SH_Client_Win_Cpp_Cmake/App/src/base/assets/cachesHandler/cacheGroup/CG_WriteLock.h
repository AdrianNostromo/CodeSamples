#pragma once

#include <base/gh.h>
#include "CG_Base.h"
#include <mutex>

namespace base {
class CG_WriteLock : public CG_Base {priv typedef CG_Base super;pub dCtor(CG_WriteLock);
    priv int isWriteLockIncremental = 0;

    std::mutex mtx{};

    pub explicit CG_WriteLock(std::string& cacheGroupId);

    prot bool getWriteLockStatus();
    pub void setWriteLockStatus(bool writeLockStatus) final;

    pub void cacheIntegrityErrorDetected() override;

    priv std::shared_ptr<IFileHandlerBlocking> getWriteLockFileFileHandlerBlocking();

    // This reads the file status only, it doesn't change local vars.
    prot bool readWriteLockFileStatusBlocking();

    pub ~CG_WriteLock() override;
};
};
