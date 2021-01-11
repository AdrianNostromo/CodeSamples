#include <desktopWindows/net/socket/util/NativeSocketCreator.h>
#include "NativeSocketsHandler.h"

using namespace desktopWindows::Net;

NativeSocketsHandler::NativeSocketsHandler()
    : super()
{
    //void
}

void NativeSocketsHandler::init(IApp* app) {
    this->app = app;
}

std::shared_ptr<base::Net::INativeSocketCreator> NativeSocketsHandler::newNativeSocketCreator(std::string& ip, int port) {
    std::shared_ptr<desktopWindows::Net::NativeSocketCreator> socketCreator = std::make_shared<desktopWindows::Net::NativeSocketCreator>(
        ip, port, app
    );

    return socketCreator;
}

NativeSocketsHandler::~NativeSocketsHandler() {
    //void
}
