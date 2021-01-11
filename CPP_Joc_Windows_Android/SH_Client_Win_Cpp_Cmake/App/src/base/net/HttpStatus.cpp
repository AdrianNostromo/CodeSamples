#include "HttpStatus.h"

using namespace base::Net;

HttpStatus::HttpStatus(int statusCode)
    : statusCode(statusCode)
{
    //void
}

int HttpStatus::getStatusCode() {
    return statusCode;
}

HttpStatus::~HttpStatus() {
    //void
}
