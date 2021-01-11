#include "CG_Base.h"
#include <base/fileSystem/Util_FileSystem.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>

using namespace base;

CG_Base::CG_Base(std::string& cacheGroupId) 
	: super(),
	cacheGroupId(cacheGroupId)
{
	//void
}

void CG_Base::cacheIntegrityErrorDetected() {
	//void
}

void CG_Base::deleteCacheBlocking() {
	std::string fPath = "cache/" + cacheGroupId + "/";
	std::shared_ptr<IFileHandlerBlocking> fhMain = Util_FileSystem::fs->getFileHandlerBlocking(fPath, Util_FileSystem::FSTarget::Private);

	fhMain->deleteIfExists();
}

CG_Base::~CG_Base() {
	//void
}
