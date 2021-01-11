#include "HttpRequestsManagerBase.h"

using namespace base;

//Request - START
HttpRequestsManagerBase::Request::Request(std::shared_ptr<base::Net::INativeHttpRequest> nativeRequest, IRequestListener* listener)
    : nativeRequest(nativeRequest), listener(listener)
{
    //void
}

void HttpRequestsManagerBase::Request::start() {
    nativeRequest->start();
}

IHttpRequestsManager::IRequestListener* HttpRequestsManagerBase::Request::getListener() {
    return listener;
}

bool HttpRequestsManagerBase::Request::getIsCanceled() {
    return isCanceled;
}

void HttpRequestsManagerBase::Request::cancel() {
    if(!isCanceled) {
        isCanceled = true;
    }
}

bool HttpRequestsManagerBase::Request::getIsFinished() {
    return nativeRequest->getIsFinished();
}

int HttpRequestsManagerBase::Request::getErrorCode() {
    return nativeRequest->getErrorCode();
}

int HttpRequestsManagerBase::Request::getResponseHttpStatusCode() {
    return nativeRequest->getResponseHttpStatusCode();
}

std::shared_ptr<std::string> HttpRequestsManagerBase::Request::getResponseBody() {
    return nativeRequest->getResponseBody();
}

HttpRequestsManagerBase::Request::~Request() {
    //void
}
//Request - END

HttpRequestsManagerBase::HttpRequestsManagerBase(IApp* app, base::Net::INativeHttpHandler* deviceHttpHandler)
    : super(),
    app(app),
    deviceHttpHandler(deviceHttpHandler)
{
    //voids
}

HttpRequestsManagerBase::~HttpRequestsManagerBase() {
    //void
}
