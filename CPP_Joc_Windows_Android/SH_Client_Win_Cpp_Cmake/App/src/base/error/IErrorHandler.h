#pragma once

#include <string>

class IErrorHandler {
public:
    virtual void handleAppCriticalError(std::string locationID, std::string msg) = 0;

	virtual ~IErrorHandler() = default;
};
