#include "Socket.h"
#include <assert.h>
#include <base/MM.h>

using namespace base::Net;

Socket::Socket(std::shared_ptr<base::Net::INativeSocket> nativeSocket)
    : nativeSocket(nativeSocket)
{
    mtxReceivedNetBuffersList = new std::mutex();
}

void Socket::reservedCreate() {
    createMain();
}

void Socket::createMain() {
    nativeSocket->setCb_onData(std::bind(&Socket::onNativeSocket_data, this, std::placeholders::_1, std::placeholders::_2));
    nativeSocket->setCb_onLogicError(std::bind(&Socket::onNativeSocket_logicError, this, std::placeholders::_1));
    nativeSocket->setCb_onNetError(std::bind(&Socket::onNativeSocket_netError, this, std::placeholders::_1));

    nativeSocket->setIsInputEnabled(true);
}

void Socket::onNativeSocket_data(
    base::Net::INativeSocket* target,
    base::Buffer& data)
{
    std::shared_ptr<base::StreamBuffer> newBuff = std::make_shared<base::StreamBuffer>(data);
    newBuff->setBoundsLimit(0);

    lockReceiveNetBuffersList();
    receivedNetBuffersList.appendReference(newBuff);
    unlockReceiveNetBuffersList();
}

void Socket::onNativeSocket_logicError(base::Net::INativeSocket* target) {
    disposeNativeSocket();

    isLogicError = true;
}

void Socket::onNativeSocket_netError(base::Net::INativeSocket* target) {
    disposeNativeSocket();

    isNetError = true;
}

void Socket::write(base::Buffer data) {
    nativeSocket->write(data);
}

bool Socket::getIsAnyError() {
    if(isLogicError || isNetError) {
        return true;
    }

    return false;
}

bool Socket::getIsLogicError() {
    return isLogicError;
}

bool Socket::getIsNetError() {
    return isNetError;
}

ArrayList<std::shared_ptr<base::StreamBuffer>>* Socket::getReceivedNetBuffersList() {
    return &receivedNetBuffersList;
}

void Socket::lockReceiveNetBuffersList() {
    mtxReceivedNetBuffersList->lock();
}

void Socket::unlockReceiveNetBuffersList() {
    mtxReceivedNetBuffersList->unlock();
}

void Socket::disposeNativeSocket() {
    if(nativeSocket != nullptr) {
        nativeSocket->reservedDisposeMain();

        nativeSocket = nullptr;
    }
}

void Socket::reservedDisposeMain() {
    if(isDisposed) {
        throw LogicException(LOC);
    }

    disposeMain();

    isDisposed = true;
}

void Socket::disposeMain() {
    disposeNativeSocket();
}

Socket::~Socket() {
    if(!isDisposed) {
        reservedDisposeMain();
    }

    if(mtxReceivedNetBuffersList != nullptr) {
        delete mtxReceivedNetBuffersList;
        mtxReceivedNetBuffersList = nullptr;
    }
}
