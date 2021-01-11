#pragma once

#include <base/gh.h>
#include "ILogHandler.h"

namespace base {
	class INativeConsole;
}

class LogHandler : virtual public ILogHandler {pub dCtor(LogHandler);
private:
	base::INativeConsole* nativeConsole;

public:
    LogHandler(base::INativeConsole* nativeConsole);
    void reservedCreate();

	void postLog(std::string locationID, std::string msg) final;
	void postLog(std::string msg) final;
	void postDebug(std::string msg) final;

	void postInfo(std::string msg) final;

	void postWarning(std::string locationID, std::string msg) final;
	void postError(std::string locationID, std::string msg) final;

	void post(LogType type, const char* fmt, va_list ap) final;
	void post_fileIO(const char* fmt, va_list ap) final;

    ~LogHandler() override;
protected:
    virtual void createMain();

};
