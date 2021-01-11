#include "CG_IndexFileHandler.h"
#include <memory>
#include <sstream>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <base/fileSystem/fileHandler/async/IFileHandlerAsync.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/exceptions/ResourceNotFoundException.h>
#include <base/exceptions/FileNotFoundException.h>
#include <base/list/ArrayList.h>
#include <base/util/StringUtil.h>
#include <base/fileSystem/IFileSystem.h>
#include <base/exceptions/DataFormatException.h>
#include <base/app/config/AppConfig.h>

using namespace base;

std::string CG_IndexFileHandler::INDEX_FILE_NAME = "__indexFile.dat";

CG_IndexFileHandler::CG_IndexFileHandler(std::string& cacheGroupId)
	: super(cacheGroupId)
{
	//void
}

void CG_IndexFileHandler::saveIndexFile() {
	Map1D<std::string, int>* entriesMap = getEntriesMap();

	// Save the number of lines on the first row.
	std::unordered_map<std::string, int>* m = entriesMap->getRawMap();
	std::shared_ptr<std::stringstream> dataSs = std::make_shared<std::stringstream>();
	
	// The first line is the entries count.
	*dataSs << entriesMap->count() << "\r\n";
	// The second line is the cache data version.
	*dataSs << AppConfig::CACHE_DATA_VERSION << "\r\n";

	// Enumerate all entries to a string stream.
	int countedEntriesCount = 0;
	for (std::unordered_map<std::string, int>::iterator it = m->begin(); it != m->end(); it++) {
		int val = it->second;
		if (val != 1) {
			throw LogicException(LOC);
		}

		*dataSs << it->first << "\r\n";

		countedEntriesCount++;
	}

	if (countedEntriesCount != entriesMap->count()) {
		throw LogicException(LOC);
	}

	// Write the index file as a text file with a .dat extension.
	std::string fPath = "cache/" + cacheGroupId + "/" + INDEX_FILE_NAME;
	std::string fPath_Tmp = fPath + ".tmp";
	std::shared_ptr<IFileHandlerAsync> fhTemp = Util_FileSystem::fs->getFileHandlerAsync(fPath_Tmp, Util_FileSystem::FSTarget::Private, this/*cacheGroup*/);

	fhTemp->createBoundedDirectories();
	fhTemp->deleteIfExists();
	fhTemp->write(dataSs);

	try {
		fhTemp->renameOrMove(fPath, true);
	}catch (ResourceNotFoundException& x) {
		throw LogicException(LOC);
	}
}

CG_IndexFileHandler::LoadIndexFileRet CG_IndexFileHandler::loadIndexFile(int targetVersionIndex) {
	std::shared_ptr<std::string> dataS;

	// Load the index file for this group as a text file.
	std::string fPath = "cache/" + cacheGroupId + "/" + INDEX_FILE_NAME;
	// Note. Using blocking here is possible because this occurs on first access only so there are no pending async operations on this cache group.
	std::shared_ptr<IFileHandlerBlocking> fh = Util_FileSystem::fs->getFileHandlerBlocking(fPath, Util_FileSystem::FSTarget::Private);
	try {
		dataS = fh->readCppStringSP();
	}catch (FileNotFoundException& x) {
		return Fail_MissingOrCorrupt;
	}
	if (dataS == nullptr) {
		throw LogicException(LOC);
	}

	// Split the ground into lines.
	std::shared_ptr<ArrayList<std::string>> linesList = StringUtil::splitToLines(*dataS, true);
	if (linesList->count() < 2) {
		return Fail_MissingOrCorrupt;
	}
	
	int processedLinesCount = 0;

	int entriesCount;
	int versionIndex;
	try {
		// The first line is the number of entries.
		entriesCount = StringUtil::parseInt(linesList->getReference(processedLinesCount++));
		if (entriesCount + 2 > linesList->count()) {
			// Not enough lines of data.
			return Fail_MissingOrCorrupt;
		}

		// The second line is the version index.
		versionIndex = StringUtil::parseInt(linesList->getReference(processedLinesCount++));
		if (versionIndex != targetVersionIndex) {
			// Wrong version, will clear the cache.
			return Fail_WrongVersion;
		}
	}catch (DataFormatException& x) {
		return Fail_MissingOrCorrupt;
	}

	// Save each non_zero_length line to the map.
	Map1D<std::string, int>* entriesMap = getEntriesMap();
	for (int i = 0; i < entriesCount; i++) {
		std::string& line = linesList->getReference(processedLinesCount++);
		if (line.length() <= 0) {
			return Fail_MissingOrCorrupt;
		}

		entriesMap->putDirect(line, 1);
	}

	return Success;
}

void CG_IndexFileHandler::onEntryChanged() {
	super::onEntryChanged();

	saveIndexFile();
}

CG_IndexFileHandler::~CG_IndexFileHandler() {
	//void
}
