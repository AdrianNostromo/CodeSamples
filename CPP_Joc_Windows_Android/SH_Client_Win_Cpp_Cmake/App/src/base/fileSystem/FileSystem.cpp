#include "FileSystem.h"
#include <base/fileSystem/fileHandler/async/FileHandlerAsyncHdd.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>

using namespace base;

FileSystem::FileSystem()
	: super()
{
	//void
}

std::shared_ptr<IFileHandlerBase> FileSystem::getFileHandler(std::string const& path, int fsTarget, int fsAccessMode) {
	if (fsAccessMode == Util_FileSystem::FSAccessMode::Blocking) {
		return getFileHandlerBlocking(path, fsTarget);
	} else if (fsAccessMode == Util_FileSystem::FSAccessMode::Async) {
		return getFileHandlerAsync(path, fsTarget);
	} else {
		throw LogicException(LOC);
	}
}

std::shared_ptr<IFileHandlerAsync> FileSystem::getFileHandlerAsync(std::string const& path, int fsTarget, base::ICacheGroup* cacheGroup) {
	std::shared_ptr<FileHandlerAsyncHdd> fHandler;
	if (fsTarget == Util_FileSystem::FSTarget::EmbeddedAssets) {
		fHandler = std::make_shared<base::FileHandlerAsyncHdd>(
			path,
			this,
			getFileHandlerBlocking(path, fsTarget),
			cacheGroup
		);
	} else if (fsTarget == Util_FileSystem::FSTarget::Private) {
		fHandler = std::make_shared<base::FileHandlerAsyncHdd>(
			path,
			this,
			getFileHandlerBlocking(path, fsTarget),
			cacheGroup
		);
	} else if (fsTarget == Util_FileSystem::FSTarget::Public) {
		// Implement when needed (my documents root or something). May be used to save screenshots.
		throw LogicException(LOC);
	} else {
		throw LogicException(LOC);
	}

	fHandler->self_weakPointer = fHandler;

	return fHandler;
}

FileSystem::~FileSystem() {
	//void
}
