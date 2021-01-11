#include "CG_WriteLock.h"
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace base;

CG_WriteLock::CG_WriteLock(std::string& cacheGroupId)
	: super(cacheGroupId)
{
	//void
}

bool CG_WriteLock::getWriteLockStatus() {
	// Use a lock in this function to finish setWriteLockStatus calls if any.
	mtx.lock();

	bool ret = isWriteLockIncremental > 0 ? true : false;

	mtx.unlock();

	return ret;
}

void CG_WriteLock::setWriteLockStatus(bool writeLockStatus) {
	mtx.lock();

	if ((!writeLockStatus && isWriteLockIncremental == 0)
		|| (writeLockStatus && isWriteLockIncremental > 100))
	{
		mtx.unlock();
		throw LogicException(LOC);
	}

	this->isWriteLockIncremental += !writeLockStatus ? -1 : 1;

	std::shared_ptr<IFileHandlerBlocking> fhMain = getWriteLockFileFileHandlerBlocking();

	if (!writeLockStatus && isWriteLockIncremental == 0) {
		// Lock counter just decremented to 0, delete the lock file.
		//GlobalAccessLogHandler::devLogHandler->post_debug("Cache (%s) write unlocked", cacheGroupId.c_str());

		fhMain->deleteIfExists();
	} else if(writeLockStatus && isWriteLockIncremental == 1) {
		// Lock counter just incremented to 1, create the lock file.
		//GlobalAccessLogHandler::devLogHandler->post_debug("Cache (%s) write locked", cacheGroupId.c_str());

		fhMain->createOrClear();
	}

	mtx.unlock();
}

void CG_WriteLock::cacheIntegrityErrorDetected() {
	super::cacheIntegrityErrorDetected();

	// No need to clear isWriteLockIncremental because it is still set correctly from the async operations that are before 
	/// the async operation to delete the cache folder.
	//void
}

std::shared_ptr<IFileHandlerBlocking> CG_WriteLock::getWriteLockFileFileHandlerBlocking() {
	std::string fPath = "cache/" + cacheGroupId + "/__cacheWriting.lock";
	std::shared_ptr<IFileHandlerBlocking> fh = Util_FileSystem::fs->getFileHandlerBlocking(fPath, Util_FileSystem::FSTarget::Private);

	return fh;
}

bool CG_WriteLock::readWriteLockFileStatusBlocking() {
	std::shared_ptr<IFileHandlerBlocking> fhMain = getWriteLockFileFileHandlerBlocking();

	bool isExists = fhMain->isExists();

	return isExists;
}

CG_WriteLock::~CG_WriteLock() {
	//void
}
