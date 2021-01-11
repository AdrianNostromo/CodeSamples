#pragma once

#include <memory>
#include <base/net/socket/util/Socket.h>
#include <base/list/ArrayList.h>
#include <base/buffer/StreamBuffer.h>
#include <base/net/socket/util/ISocket.h>
#include <base/net/buffer/NetMsgsBlock.h>

namespace base {
namespace Net {
class BasicTcpConnection {
protected:
    std::shared_ptr<ISocket> socket = nullptr;

    std::function<void(
        base::Net::BasicTcpConnection* target)> cb_onLogicError;
    std::function<void(
        base::Net::BasicTcpConnection* target)> cb_onNetError;

public:
    BasicTcpConnection(BasicTcpConnection const&) = delete;
    BasicTcpConnection(BasicTcpConnection &&) = default;
    BasicTcpConnection& operator=(BasicTcpConnection const&) = delete;
    BasicTcpConnection& operator=(BasicTcpConnection &&) = default;

    explicit BasicTcpConnection(std::shared_ptr<Socket>& socket);

    bool isData();

    void setCb_onLogicError(std::function<void(
        base::Net::BasicTcpConnection* target)> cb_onLogicError);
    void clearCb_onLogicError();
    void setCb_onNetError(std::function<void(
        base::Net::BasicTcpConnection* target)> cb_onNetError);
    void clearCb_onNetError();

    void reservedDisposeMain();
    virtual ~BasicTcpConnection();
protected:
    virtual void disposeMain();

};
};
};
