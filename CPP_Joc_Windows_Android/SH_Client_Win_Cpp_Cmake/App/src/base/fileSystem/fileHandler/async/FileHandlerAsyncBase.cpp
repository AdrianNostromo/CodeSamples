#include "FileHandlerAsyncBase.h"
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>

using namespace base;

FileHandlerAsyncBase::FileHandlerAsyncBase(std::string const& path, base::ICacheGroup* cacheGroup)
	: super(path),
	cacheGroup(cacheGroup)
{
	//void
}

void FileHandlerAsyncBase::cacheGroupWriteLockingOperationsCountChange(bool isLockOperationAdded) {
	if (cacheGroup == nullptr) {
		return;
	}

	if ((!isLockOperationAdded && cacheGroupWriteLockingOperationsCount == 0)
		|| (isLockOperationAdded && cacheGroupWriteLockingOperationsCount > 100))
	{
		throw LogicException(LOC);
	}

	this->cacheGroupWriteLockingOperationsCount += !isLockOperationAdded ? -1 : 1;

	if (!isLockOperationAdded && cacheGroupWriteLockingOperationsCount == 0) {
		// Counter just decremented to 0. Release the lock.
		cacheGroup->setWriteLockStatus(false);
	} else if (isLockOperationAdded && cacheGroupWriteLockingOperationsCount == 1) {
		// Counter just incremented to 1. Grab the lock.
		// Note. The lock is instantly set when the async action is added and not when processed to avoid lock toggle between separate async fHandlers processing. This method will keep only 1 lock active until all async fHandlers operations finish.
		cacheGroup->setWriteLockStatus(true);
	}
}

FileHandlerAsyncBase::~FileHandlerAsyncBase() {
	//void
}
