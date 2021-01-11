#include "NativeHttpRequestBase.h"
#include <base/exceptions/LogicException.h>

using namespace desktopWindows::Net;

NativeHttpRequestBase::NativeHttpRequestBase(std::string& httpMethod, std::string& baseUrl, std::string& urlParams, IApp* app)
    : httpMethod(httpMethod), baseUrl(baseUrl), urlParams(urlParams), app(app)
{
    //void
}

void NativeHttpRequestBase::start() {
    if(isRequested_start) {
        throw LogicException(LOC);
    }

    isRequested_start = true;
}

void NativeHttpRequestBase::cancel() {
    if(isRequested_cancel) {
        throw LogicException(LOC);
    }

    isRequested_cancel = true;
}

int NativeHttpRequestBase::getErrorCode() {
    return errorCode;
}

int NativeHttpRequestBase::getResponseHttpStatusCode() {
    return responseHttpStatusCode;
}

std::shared_ptr<std::string> NativeHttpRequestBase::getResponseHeader() {
    return responseHeader;
}

std::shared_ptr<std::string> NativeHttpRequestBase::getResponseBody() {
    return responseBody;
}

NativeHttpRequestBase::~NativeHttpRequestBase() {
    if(responseHeader != nullptr) {
        responseHeader = nullptr;
    }
    if(responseBody != nullptr) {
        responseBody = nullptr;
    }
}
