#include "CH_Entries.h"
#include <base/app/config/AppConfig.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace base;
using namespace base::crashHandler;

CH_Entries::CH_Entries()
	: super()
{
	//void
}

void CH_Entries::create() {
	super::create();

	appVersion = AppConfig::APP_VERSION;
	startTimestamp = std::chrono::system_clock::now();
}

void CH_Entries::pushEntry_Info(const char* fileName, int lineIndex, std::string msg) {
	entriesList.appendEmplace(
		CrashLogEntry::Type::Info, std::chrono::system_clock::now(),
		fileName, lineIndex, msg
	);
}

void CH_Entries::pushEntry_CriticalException(const char* fileName, int lineIndex, std::string msg) {
	GlobalAccessLogHandler::devLogHandler->post_debug("CriticalException in file %s on line %i", fileName, lineIndex);

	entriesList.appendEmplace(
		CrashLogEntry::Type::CriticalException, std::chrono::system_clock::now(),
		fileName, lineIndex, msg
	);
}

CH_Entries::~CH_Entries() {
	//void
}
