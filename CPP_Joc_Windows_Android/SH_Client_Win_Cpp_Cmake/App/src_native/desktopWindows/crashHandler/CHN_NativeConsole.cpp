#include "CHN_NativeConsole.h"
#include <iostream>
#include <vendor/date/date.h>

using namespace desktopWindows;

CHN_NativeConsole::CHN_NativeConsole()
	: super()
{
	//void
}

void CHN_NativeConsole::consoleOutputErrorLogs() {
	// This is required for pretty iostream printing std::chrono values.
	using namespace date;
	std::cout << "<CrashLog> Start: appVersion:" << appVersion << ", startTimestamp:" << startTimestamp << ", totalEntriesCount:" << entriesList.count() << std::endl;

	int ct = 0;
	for (ListDL<CrashLogEntry>::Entry* entry = entriesList.getFirst(); entry != nullptr; entry = entry->next) {
		if (entry->data.type == CrashLogEntry::Type::CriticalException) {
			auto autoTimeFromStartMs = std::chrono::duration_cast<std::chrono::milliseconds>(entry->data.timestamp - startTimestamp);
			long timeFromStartMs = static_cast<int>(autoTimeFromStartMs.count());

			std::cout << "<CrashLog> index:" << ct << ", type:" << "CriticalException" << ", file:" << entry->data.fileName << ", line:" << entry->data.lineIndex << ", msg:" << entry->data.msg << ", timeFromStartMs:" << timeFromStartMs << std::endl;
		}

		ct++;
	}

	std::cout << "<CrashLog> End." << std::endl;
}

CHN_NativeConsole::~CHN_NativeConsole() {
	//void
}
