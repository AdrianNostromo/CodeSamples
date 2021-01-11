#include "BasicTcpConnection.h"
#include <base/net/exception/NetConException.h>
#include <base/exceptions/IOException.h>
#include <assert.h>
#include <functional>

using namespace base::Net;

BasicTcpConnection::BasicTcpConnection(std::shared_ptr<Socket>& socket)
    : socket(socket)
{
    //void
}

bool BasicTcpConnection::isData() {
    if(socket->getReceivedNetBuffersList()->count() > 0) {
        return true;
    }

    return false;
}

void BasicTcpConnection::setCb_onLogicError(std::function<void(
    base::Net::BasicTcpConnection* target)> cb_onLogicError)
{
    this->cb_onLogicError = cb_onLogicError;
}

void BasicTcpConnection::clearCb_onLogicError() {
    this->cb_onLogicError = nullptr;
}

void BasicTcpConnection::setCb_onNetError(std::function<void(
    base::Net::BasicTcpConnection* target)> cb_onNetError)
{
    this->cb_onNetError = cb_onNetError;
}

void BasicTcpConnection::clearCb_onNetError() {
    this->cb_onNetError = nullptr;
}

void BasicTcpConnection::reservedDisposeMain() {
    disposeMain();
}

void BasicTcpConnection::disposeMain() {
    if (socket != nullptr) {
        socket->reservedDisposeMain();

        socket = nullptr;
    }
}

BasicTcpConnection::~BasicTcpConnection() {
    //void
}
