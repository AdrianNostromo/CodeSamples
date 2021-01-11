#pragma once

#include <base/gh.h>
#include <base/fileSystem/fileHandler/FileHandlerBase.h>
#include <base/fileSystem/fileHandler/async/IFileHandlerAsync.h>
#include <memory>

namespace base {
    class ICacheGroup;
};

namespace base {
class FileHandlerAsyncBase : public FileHandlerBase, virtual public IFileHandlerAsync {priv typedef FileHandlerBase super;pub dCtor(FileHandlerAsyncBase);
    // Optional, only used for uperations created by a cache group.
    base::ICacheGroup* cacheGroup;

    // These are used to make sure this fHandler is not deleted while operations are in progress (delete could occur if the user doesn't save the shared_pointer of this.
    // Can't initialise to nullptr, mjust leave it unitialised at start.
    pub std::weak_ptr<IFileHandlerAsync> self_weakPointer{};

    // This is used to set,clear the write lock on the cache group as needed.
    priv int cacheGroupWriteLockingOperationsCount = 0;

    pub explicit FileHandlerAsyncBase(std::string const& path, base::ICacheGroup* cacheGroup);
    
    pub void cacheGroupWriteLockingOperationsCountChange(bool isLockOperationAdded) final;

    pub ~FileHandlerAsyncBase() override;
};
};
