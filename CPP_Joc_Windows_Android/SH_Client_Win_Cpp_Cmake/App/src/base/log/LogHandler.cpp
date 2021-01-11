#include "LogHandler.h"
#include "GlobalAccessLogHandler.h"
#include <iostream>
#include <base/app/config/AppConfig.h>
#include <base/console/INativeConsole.h>
#include <base/exceptions/LogicException.h>

//asdB;// Dispatch events that are captured by a error recovery handler and log U.I.;

LogHandler::LogHandler(base::INativeConsole* nativeConsole)
	: nativeConsole(nativeConsole)
{
	GlobalAccessLogHandler::devLogHandler = this;
}

void LogHandler::reservedCreate() {
	createMain();
}

void LogHandler::createMain() {
    //void
}

void LogHandler::postLog(std::string locationID, std::string msg) {
    if(AppConfig::IS_DEBUG) {
        std::cout << "Dev-Log(location: " << locationID << "): " << msg << std::endl;
    }
}

void LogHandler::postLog(std::string msg) {
    if(AppConfig::IS_DEBUG) {
        std::cout << "Dev-Log: " << msg << std::endl;
    }
}

void LogHandler::postDebug(std::string msg) {
    if(AppConfig::IS_DEBUG) {
        std::cout << "Dev-Debug: " << msg << std::endl;
    }
}

void LogHandler::postInfo(std::string msg) {
    if(AppConfig::IS_DEBUG) {
        std::cout << "Dev-Info: " << msg << std::endl;
    }
}

void LogHandler::postWarning(std::string locationID, std::string msg) {
    if(AppConfig::IS_DEBUG) {
        std::cout << "Dev-Warning(location: " << locationID << "): " << msg << std::endl;
    }
}

void LogHandler::postError(std::string locationID, std::string msg) {
    if(AppConfig::IS_DEBUG) {
        std::cout << "Dev-Error(location: " << locationID << "): " << msg << std::endl;
    }
}

void LogHandler::post(LogType type, const char* fmt, va_list ap) {
    if (type == DEBUG) {
        if (AppConfig::IS_DEBUG) {
            nativeConsole->post_debug(fmt, ap);
        }
    } else if (type == WARNING) {
        nativeConsole->post_warn(fmt, ap);
    } else {
        // Unhandled log type.
        throw LogicException(LOC);
    }
}

void LogHandler::post_fileIO(const char* fmt, va_list ap) {
	nativeConsole->post_info(fmt, ap);
}

LogHandler::~LogHandler() {
    //void
}
