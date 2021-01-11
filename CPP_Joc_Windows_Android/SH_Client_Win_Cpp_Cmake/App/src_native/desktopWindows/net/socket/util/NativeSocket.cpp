#include "NativeSocket.h"
#include <thread>
#include <base/exceptions/LogicException.h>
#include <base/exceptions/IOException.h>
#include <sys/types.h>
#include <cassert>
#include <functional>
#include <base/app/IApp.h>
#include <base/threadsManager/IThreadsManager.h>

using namespace desktopWindows::Net;

NativeSocket::NativeSocket(int socketFd, IApp* app)
    : socketFd(socketFd), app(app)
{
    //void
}

void NativeSocket::reservedCreate() {
    createMain();
}

void NativeSocket::createMain() {
    managedThread_in = app->getThreadsManager()->newThread("NativeSocket_in", std::bind(&NativeSocket::logicLoop_in, this, std::placeholders::_1));
    managedThread_out = app->getThreadsManager()->newThread("NativeSocket_out", std::bind(&NativeSocket::logicLoop_out, this, std::placeholders::_1));
}

void NativeSocket::setIsInputEnabled(bool isInputEnabled) {
    this->isInputEnabled = isInputEnabled;
}

void NativeSocket::write(base::Buffer data) {
    writeCache.appendReference(data);
}

void NativeSocket::setCb_onData(std::function<void(
    base::Net::INativeSocket* target,
    base::Buffer& data)> cb_onData)
{
    this->cb_onData = cb_onData;
}

void NativeSocket::setCb_onLogicError(std::function<void(
    base::Net::INativeSocket* target)> cb_onLogicError)
{
    this->cb_onLogicError = cb_onLogicError;
}

void NativeSocket::setCb_onNetError(std::function<void(
    base::Net::INativeSocket* target)> cb_onNetError)
{
    this->cb_onNetError = cb_onNetError;
}

unsigned int NativeSocket::logicLoop_in(base::IManagedThread* managedThread) {
    throw LogicException(LOC);
	//asdA10;
 //   while(!isDisposed && !isError) {
 //       if(isInputEnabled) {
 //           int bytesAvailable = -1;
 //           ssize_t bytesRead = 0;

 //           try {
 //               bytesAvailable = available();
 //               if (bytesAvailable > 0) {
 //                   dataBuff = new char[bytesAvailable];
 //                   bytesRead = read(socketFd, dataBuff, static_cast<size_t>(bytesAvailable));
 //               }
 //           } catch (IOException &x) {
 //               isError = true;

 //               if (dataBuff != nullptr) {
 //                   delete[] dataBuff;

 //                   dataBuff = nullptr;
 //               }
 //               if (!isDisposed) {
 //                   cb_onNetError(this);
 //               }

 //               return;
 //           }

 //           if (dataBuff != nullptr) {
 //               if (bytesRead != bytesAvailable) {
 //                   isError = true;

 //                   if (!isDisposed && cb_onLogicError) {
 //                       cb_onLogicError(this);
 //                   }

 //                   return;
 //               }

 //               base::Buffer data = std::make_shared<base::Buffer>(bytesAvailable, dataBuff, true);
 //               dataBuff = nullptr;

 //               if (!isDisposed) {
 //                   if (!cb_onData) {
 //                       isError = true;

 //                       if (!isDisposed && cb_onLogicError) {
 //                           cb_onLogicError(this);
 //                       }

 //                       return;
 //                   }

 //                   cb_onData(this, data);
 //               }
 //           }
 //       }

 //       if (isDisposed || isError) {
 //           break;
 //       }

 //       std::this_thread::sleep_for(std::chrono::milliseconds(5));//asd_x;// don't sleep here any more with the new threads manager. Also don't use a loop here.
 //   }

 //   return 5/*sleepDurationMs*/;
}

unsigned int NativeSocket::logicLoop_out(base::IManagedThread* managedThread) {
    throw LogicException(LOC);
    //asdA9;
    //while (!isDisposed && !isError) {
    //    while (writeCache.size() > 0) {
    //        base::Buffer dataBlock = writeCache.getDirect(0);

    //        ssize_t bytesSent = send(socketFd, dataBlock->getBuffer(), dataBlock->getBytesCount(), 0);
    //        if(bytesSent != dataBlock->getBytesCount()) {
    //            isError = true;

    //            if(!isDisposed) {
    //                cb_onLogicError(this);
    //            }
    //        }

    //        writeCache.remove(0);

    //        if(isDisposed || isError) {
    //            break;
    //        }

    //        std::this_thread::sleep_for(std::chrono::milliseconds(5));//asd_x;// don't sleep here any more with the new threads manager. Also don't use a loop here.
    //    }
    //}
    //
    //return 5/*sleepDurationMs*/;
}

int NativeSocket::available() {
	return 0;
	/*asdA9;
    int count;
    ioctl(socketFd, FIONREAD, &count);

    if(count < 0) {
        throw IOException();
    }

    return count;*/
}

void NativeSocket::reservedDisposeMain() {
    if(isDisposed) {
        throw LogicException(LOC);
    }

    disposeMain();

    isDisposed = true;
}

void NativeSocket::disposeMain() {
	/*asdA9;
    if(socketFd >= 0) {
        shutdown(socketFd, SHUT_RDWR);
        close(socketFd);

        socketFd = -1;
    }*/
}

NativeSocket::~NativeSocket() {
    if(!isDisposed) {
        reservedDisposeMain();
    }

    //void
}
