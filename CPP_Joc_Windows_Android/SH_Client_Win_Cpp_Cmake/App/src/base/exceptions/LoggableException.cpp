#include "LoggableException.h"
#include <base/crashHandler/ICrashHandler.h>

LoggableException::LoggableException(const char* exceptionType, const char* fileName, size_t lineIndex)
    : Exception()
{
    base::ICrashHandler::crashHandler->pushEntry_CriticalException(
        fileName, lineIndex, exceptionType
    );
}
