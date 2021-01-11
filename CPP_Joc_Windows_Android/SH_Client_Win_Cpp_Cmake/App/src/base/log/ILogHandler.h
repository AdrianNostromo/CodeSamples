#pragma once

#include <base/gh.h>
#include <string>
#include <cstdarg>

class ILogHandler {
virtual void postLog(std::string locationID, std::string msg) = 0;
	pub enum LogType { DEBUG, WARNING };

    pub virtual void postLog(std::string msg) = 0;
	pub virtual void postDebug(std::string msg) = 0;

	pub virtual void postInfo(std::string msg) = 0;

	pub virtual void postWarning(std::string locationID, std::string msg) = 0;

	pub virtual void postError(std::string locationID, std::string msg) = 0;

	pub virtual void post(LogType type, const char* fmt, va_list ap) = 0;
	
	pub virtual void post_fileIO(const char* fmt, va_list ap) = 0;

	pub void post(LogType type, const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		post(type, fmt, args);
		va_end(args);
	}

	pub void post_debug(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		post(LogType::DEBUG, fmt, args);
		va_end(args);
	}

	pub void post_warning(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		post(LogType::WARNING, fmt, args);
		va_end(args);
	}

	pub void post_fileIO(const char* fmt, ...) {
		va_list args;
		va_start(args, fmt);
		post_fileIO(fmt, args);
		va_end(args);
	}

	pub virtual ~ILogHandler() = default;

};
