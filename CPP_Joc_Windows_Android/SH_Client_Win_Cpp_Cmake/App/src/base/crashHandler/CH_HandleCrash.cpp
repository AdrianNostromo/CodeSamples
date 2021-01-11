#include "CH_HandleCrash.h"

using namespace base;
using namespace base::crashHandler;

CH_HandleCrash::CH_HandleCrash()
	: super()
{
	//void
}

void CH_HandleCrash::handleCrash() {
	consoleOutputErrorLogs();
	writeCrashLogs();
}

CH_HandleCrash::~CH_HandleCrash() {
	//void
}
