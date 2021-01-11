#include "CHN_FileSystem.h"
#include <vendor/date/date.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <chrono>

using namespace desktopWindows;

CHN_FileSystem::CHN_FileSystem()
	: super()
{
	//void
}

void CHN_FileSystem::writeCrashLogs() {
	std::stringstream ss{};
	{
		long startTimestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(startTimestamp.time_since_epoch()).count();

		ss << "CrashLog_" << startTimestampMs << ".log";
	}
	std::string fName = ss.str();
	std::ofstream oFile(fName, std::ofstream::out);

	using namespace date;
	oFile << "<CrashLog> Start: appVersion:" << appVersion << ", startTimestamp:" << startTimestamp << ", totalEntriesCount:" << entriesList.count() << std::endl;

	int ct = 0;
	for (ListDL<CrashLogEntry>::Entry* entry = entriesList.getFirst(); entry != nullptr; entry = entry->next) {
		std::string logEntryType = "";
		if (entry->data.type == CrashLogEntry::Type::CriticalException) {
			logEntryType = "CriticalException";
		}else if (entry->data.type == CrashLogEntry::Type::Info) {
			logEntryType = "Info";
		} else {
			assert(false);
		}

		auto autoTimeFromStartMs = std::chrono::duration_cast<std::chrono::milliseconds>(entry->data.timestamp - startTimestamp);
		long timeFromStartMs = static_cast<int>(autoTimeFromStartMs.count());

		oFile << "<CrashLog> index:" << ct << ", type:" << "CriticalException" << ", file:" << entry->data.fileName << ", line:" << entry->data.lineIndex << ", msg:" << entry->data.msg << ", timeFromStartMs:" << timeFromStartMs << std::endl;

		ct++;
	}

	oFile << "<CrashLog> End." << std::endl;

	oFile.close();
}

CHN_FileSystem::~CHN_FileSystem() {
	//void
}
