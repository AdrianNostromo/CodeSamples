#pragma once

#include <base/log/ILogHandler.h>
#include "IErrorHandler.h"

class ErrorHandler : virtual public IErrorHandler {
public:
    ILogHandler* logger;

public:
    ErrorHandler(ILogHandler* logger);
    void reservedCreate();

    void handleAppCriticalError(std::string locationID, std::string msg) override;

    ~ErrorHandler() override;
protected:
    virtual void createMain();

};
