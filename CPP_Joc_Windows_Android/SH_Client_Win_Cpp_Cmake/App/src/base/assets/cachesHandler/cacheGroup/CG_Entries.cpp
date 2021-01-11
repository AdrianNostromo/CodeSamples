#include "CG_Entries.h"
#include <base/fileSystem/fileHandler/async/IFileHandlerAsync.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/exceptions/ResourceNotFoundException.h>

using namespace base;

CG_Entries::CG_Entries(std::string& cacheGroupId)
	: super(cacheGroupId)
{
	//void
}

Map1D<std::string, int>* CG_Entries::getEntriesMap() {
	return &entriesMap;
}

bool CG_Entries::isCache(std::string& entryId) {
	return entriesMap.containsKey(entryId);
}

void CG_Entries::setCache(std::string& entryId, base::Buffer data) {
	std::string fPath = "cache/" + cacheGroupId + "/" + entryId;
	std::string fPath_Tmp = fPath + ".tmp";
	std::shared_ptr<IFileHandlerAsync> fhTemp = Util_FileSystem::fs->getFileHandlerAsync(
		fPath_Tmp, Util_FileSystem::FSTarget::Private,
		this/*cacheGroup*/
	);

	fhTemp->createBoundedDirectories();
	fhTemp->deleteIfExists();
	fhTemp->write(data);

	try {
		fhTemp->renameOrMove(fPath, true);
	}catch (ResourceNotFoundException& x) {
		throw LogicException(LOC);
	}

	setCache(entryId);
}

void CG_Entries::setCache(std::string& entryId, std::shared_ptr<std::string> data) {
	std::string fPath = "cache/" + cacheGroupId + "/" + entryId;
	std::string fPath_Tmp = fPath + ".tmp";
	std::shared_ptr<IFileHandlerAsync> fhTemp = Util_FileSystem::fs->getFileHandlerAsync(
		fPath_Tmp, Util_FileSystem::FSTarget::Private,
		this/*cacheGroup*/
	);

	fhTemp->createBoundedDirectories();
	fhTemp->deleteIfExists();
	fhTemp->write(data);

	try {
		fhTemp->renameOrMove(fPath, true);
	}
	catch (ResourceNotFoundException& x) {
		throw LogicException(LOC);
	}

	setCache(entryId);
}

void CG_Entries::setCache(std::string& entryId, sp<std::string> data) {
	std::string fPath = "cache/" + cacheGroupId + "/" + entryId;
	std::string fPath_Tmp = fPath + ".tmp";
	std::shared_ptr<IFileHandlerAsync> fhTemp = Util_FileSystem::fs->getFileHandlerAsync(
		fPath_Tmp, Util_FileSystem::FSTarget::Private,
		this/*cacheGroup*/
	);

	fhTemp->createBoundedDirectories();
	fhTemp->deleteIfExists();
	fhTemp->write(data);

	try {
		fhTemp->renameOrMove(fPath, true);
	}
	catch (ResourceNotFoundException& x) {
		throw LogicException(LOC);
	}

	setCache(entryId);
}

void CG_Entries::setCache(std::string& entryId, std::shared_ptr<std::stringstream> data) {
	std::string fPath = "cache/" + cacheGroupId + "/" + entryId;
	std::string fPath_Tmp = fPath + ".tmp";
	std::shared_ptr<IFileHandlerAsync> fhTemp = Util_FileSystem::fs->getFileHandlerAsync(
		fPath_Tmp, Util_FileSystem::FSTarget::Private,
		this/*cacheGroup*/
	);

	fhTemp->createBoundedDirectories();
	fhTemp->deleteIfExists();
	fhTemp->write(data);

	try {
		fhTemp->renameOrMove(fPath, true);
	}
	catch (ResourceNotFoundException& x) {
		throw LogicException(LOC);
	}

	setCache(entryId);
}

void CG_Entries::setCache(std::string& entryId) {
	if (isCache(entryId)) {
		return;
	}

	entriesMap.putDirect(entryId, 1);

	onEntryChanged();
}

bool CG_Entries::clearCache(std::string& entryId) {
	return entriesMap.removeOptional(entryId);
}

std::shared_ptr<IFileHandlerAsync> CG_Entries::getCacheFileHandlerAsync(std::string& entryId) {
	if (!isCache(entryId)) {
		return nullptr;
	}

	std::string fPath = "cache/" + cacheGroupId + "/" + entryId;
	std::shared_ptr<IFileHandlerAsync> fhMain = Util_FileSystem::fs->getFileHandlerAsync(
		fPath, Util_FileSystem::FSTarget::Private,
		this/*cacheGroup*/
	);

	return fhMain;
}

void CG_Entries::cacheIntegrityErrorDetected() {
	super::cacheIntegrityErrorDetected();

	// Delete the cache folder.
	std::string fPath = "cache/" + cacheGroupId + "/";
	// Don't set the cache group because the entire folder will be deleted and the file should not be used.
	std::shared_ptr<IFileHandlerAsync> fhMain = Util_FileSystem::fs->getFileHandlerAsync(
		fPath, Util_FileSystem::FSTarget::Private,
		nullptr/*cacheGroup*/
	);

	fhMain->deleteIfExists();

	// Clear the cache map. This also causes the index to be saved.
	entriesMap.clear();
	onEntryChanged();
}

void CG_Entries::onEntryChanged() {
	//void
}

CG_Entries::~CG_Entries() {
	//void
}
