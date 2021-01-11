#pragma once

#include <base/gh.h>
#include "FileSystemAsyncOperationsQueue.h"

namespace base {
class FileSystem : public FileSystemAsyncOperationsQueue {priv typedef FileSystemAsyncOperationsQueue super;pub dCtor(FileSystem);
    pub explicit FileSystem();

	pub std::shared_ptr<IFileHandlerBase> getFileHandler(std::string const& path, int fsTarget, int fsAccessMode) override;
	pub std::shared_ptr<IFileHandlerAsync> getFileHandlerAsync(std::string const& path, int fsTarget, base::ICacheGroup* cacheGroup=nullptr) override;

    pub ~FileSystem() override;
};
};
