#pragma once

#include <base/gh.h>
#include <base/net/socket/util/INativeSocket.h>
#include <functional>
#include <memory>
#include <base/list/ArrayList.h>

class IApp;
namespace base {
    class IManagedThread;
};

namespace desktopWindows {
namespace Net {
class NativeSocket : virtual public base::Net::INativeSocket {pub dCtor(NativeSocket);
    priv bool isDisposed = false;
    priv bool isError = false;

    priv int socketFd;

    prot IApp* app = nullptr;

    priv bool isInputEnabled = false;

    priv char* dataBuff = nullptr;

    priv base::IManagedThread* managedThread_in = nullptr;
    priv base::IManagedThread* managedThread_out = nullptr;

    priv ArrayList<base::Buffer> writeCache{};

    priv std::function<void(
        base::Net::INativeSocket* target,
        base::Buffer& data)> cb_onData;
    priv std::function<void(
        base::Net::INativeSocket* target)> cb_onLogicError;
    priv std::function<void(
        base::Net::INativeSocket* target)> cb_onNetError;

    pub explicit NativeSocket(int socketFd, IApp* app);
    pub void reservedCreate();

    pub void setIsInputEnabled(bool isInputEnabled) override;

    pub void write(base::Buffer data) override;

    pub void setCb_onData(std::function<void(
        base::Net::INativeSocket* target,
        base::Buffer& data)> cb_onData) override;
    pub void setCb_onLogicError(std::function<void(
        base::Net::INativeSocket* target)> cb_onLogicError) override;
    pub void setCb_onNetError(std::function<void(
        base::Net::INativeSocket* target)> cb_onNetError) override;

    pub unsigned int logicLoop_in(base::IManagedThread* managedThread);
    pub unsigned int logicLoop_out(base::IManagedThread* managedThread);

    prot virtual void createMain();

    prot virtual void disposeMain();

    priv int available();

    pub void reservedDisposeMain() final;
    pub ~NativeSocket() override;
};
};
};
