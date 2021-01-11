#include "NativeHttpHandler.h"
#include <assert.h>
#include <desktopWindows/net/http/util/NativeHttpRequest.h>

using namespace desktopWindows::Net;

NativeHttpHandler::NativeHttpHandler()
    : super()
{
    //void;
}

void NativeHttpHandler::init(IApp* app) {
    this->app = app;
}

std::shared_ptr<base::Net::INativeHttpRequest> NativeHttpHandler::createRequest(std::string& httpMethod, std::string& baseUrl, std::string& urlParams) {
    std::shared_ptr<desktopWindows::Net::NativeHttpRequest> newRequest = std::make_shared<desktopWindows::Net::NativeHttpRequest>(
        httpMethod, baseUrl, urlParams,
        app
    );

    return newRequest;
}

NativeHttpHandler::~NativeHttpHandler() {
    //void;
}
