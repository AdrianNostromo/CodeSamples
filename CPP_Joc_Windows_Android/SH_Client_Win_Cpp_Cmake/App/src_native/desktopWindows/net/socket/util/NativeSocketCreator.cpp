#include "NativeSocketCreator.h"
#include "NativeSocket.h"
#include <base/exceptions/LogicException.h>
#include <sys/types.h>
#include <string.h>
#include <base/app/IApp.h>
#include <base/threadsManager/IThreadsManager.h>
#include <base/threadsManager/managedThread/IManagedThread.h>

using namespace desktopWindows::Net;

NativeSocketCreator::NativeSocketCreator(std::string& ip, int port, IApp* app)
    : ip(ip), port(port), app(app)
{
    //void
}

void NativeSocketCreator::start() {
    if (isRequested_start) {
        throw LogicException(LOC);
    }

    isRequested_start = true;

    if (status != Status::Prestart) {
        throw LogicException(LOC);
    }

    managedThread = app->getThreadsManager()->newThread("NativeSocketCreator", std::bind(&NativeSocketCreator::logicLoop, this, std::placeholders::_1));
}

unsigned int NativeSocketCreator::logicLoop(base::IManagedThread* managedThread) {
    // Wait for the start command to be received.
    if(!isRequested_start && !isRequested_cancel) {
        totalStartWaitTimeS += 0.1f;
        if(totalStartWaitTimeS >= 5.0f) {
            // Request never started. Logic error.

            managedThread->removeManaged();
            managedThread = nullptr;

            throw LogicException(LOC);
        }

        return 50;//sleepDurationMs
    }

    managedThread->removeManaged();
    managedThread = nullptr;

    status = Status::InProgress;

    if(errorCode == ErrorCode::None && !isRequested_cancel && status == Status::InProgress) {
        createSocket();
    }

    if(status == 0) {
        throw LogicException(LOC);
    }

    if(status != Status::InProgress) {
        throw LogicException(LOC);
    }

    if (isRequested_cancel) {
        // This code overrides error codes.
        errorCode = ErrorCode::Canceled;
    }

    status = Status::Finished;

    return 0;
}

void NativeSocketCreator::createSocket() {
	/*asdA9;
    socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFd < 0) {
        errorCode = ErrorCode::NetError;

        return ;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(static_cast<uint16_t>(port));

    int r = inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);
    if(r != 1) {
        shutdown(socketFd, SHUT_RDWR);
        close(socketFd);
        socketFd = -1;

        if(r == 0) {
            errorCode = ErrorCode::LogicError;
        }else {
            errorCode = ErrorCode::NetError;
        }

        return ;
    }

    r = connect(socketFd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(r != 0) {
        shutdown(socketFd, SHUT_RDWR);
        close(socketFd);
        socketFd = -1;

        errorCode = ErrorCode::NetError;

        return ;
    }*/
}

void NativeSocketCreator::cancel() {
    if(isRequested_cancel) {
        throw LogicException(LOC);
    }

    isRequested_cancel = true;
}

bool NativeSocketCreator::getIsFinished() {
    if(status == Status::Finished) {
        return true;
    }

    return false;
}

int NativeSocketCreator::getErrorCode() {
    return errorCode;
}

std::shared_ptr<base::Net::INativeSocket> NativeSocketCreator::extractNativeSocket() {
    if(status != Status::Finished || errorCode != ErrorCode::None || socketFd < 0) {
        throw LogicException(LOC);
    }

    std::shared_ptr<desktopWindows::Net::NativeSocket> sock = std::make_shared<desktopWindows::Net::NativeSocket>(
        socketFd,
        app
    );
    sock->reservedCreate();

    socketFd = -1;

    return sock;
}

void NativeSocketCreator::closeSocket() {
	//asdA9;
 //   if(socketFd >= 0) {
 //       // Close the socket.
 //       shutdown(socketFd, SHUT_RDWR);
 //       close(socketFd);

 //       socketFd = -1;
 //   }
}

NativeSocketCreator::~NativeSocketCreator() {
    if(socketFd >= 0) {
        closeSocket();
    }
}
