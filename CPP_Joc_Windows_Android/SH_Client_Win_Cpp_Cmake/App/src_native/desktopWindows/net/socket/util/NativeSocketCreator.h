#pragma once

#include <base/gh.h>
#include <base/net/socket/util/INativeSocketCreator.h>
#include <string>

class IApp;
namespace base {
    class IManagedThread;
};

namespace desktopWindows {
namespace Net {
class NativeSocketCreator : virtual public base::Net::INativeSocketCreator {pub dCtor(NativeSocketCreator);
    priv class Status {
        pub static const int Prestart = 0;
        pub static const int InProgress = 1;
        pub static const int Finished = 2;
    };

    priv std::string ip;
    priv int port;

    prot IApp* app;

    priv base::IManagedThread* managedThread = nullptr;

    priv bool isRequested_start = false;
    priv bool isRequested_cancel = false;

    priv int status = Status::Prestart;
    priv int errorCode = ErrorCode::None;

    priv int socketFd = -1;

    priv float totalStartWaitTimeS = 0.0f;

    pub explicit NativeSocketCreator(std::string& ip, int port, IApp* app);

    pub unsigned int logicLoop(base::IManagedThread* managedThread);

    pub void createSocket();
    pub void start() override;
    pub void cancel() override;

    pub bool getIsFinished() override;
    pub int getErrorCode() override;

    pub std::shared_ptr<base::Net::INativeSocket> extractNativeSocket() override;

    pub void closeSocket();

    pub ~NativeSocketCreator() override;
};
};
};
