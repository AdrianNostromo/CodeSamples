#include "ErrorHandler.h"

ErrorHandler::ErrorHandler(ILogHandler* logger)
    :logger(logger)
{
    //void
}

void ErrorHandler::reservedCreate() {
    createMain();
}

void ErrorHandler::createMain() {
    //void
}

void ErrorHandler::handleAppCriticalError(std::string locationID, std::string msg) {
    logger->postError(locationID, msg);

    //asdZ;// handle this. Maybe show a popup or a error_icon in a corner.
}

ErrorHandler::~ErrorHandler() {
    //void
}
