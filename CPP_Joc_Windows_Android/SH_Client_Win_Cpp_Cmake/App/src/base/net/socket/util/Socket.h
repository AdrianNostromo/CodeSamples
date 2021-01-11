#pragma once

#include "INativeSocket.h"
#include "ISocket.h"
#include <functional>
#include <memory>
#include <mutex>

namespace base {
namespace Net {
class Socket : virtual public ISocket {
private:
    std::shared_ptr<base::Net::INativeSocket> nativeSocket;

    bool isLogicError = false;
    bool isNetError = false;

    ArrayList<std::shared_ptr<base::StreamBuffer>> receivedNetBuffersList{};
    std::mutex* mtxReceivedNetBuffersList;

    bool isDisposed = false;

public:
    Socket(Socket const&) = delete;
    Socket(Socket &&) = default;
    Socket& operator=(Socket const&) = delete;
    Socket& operator=(Socket &&) = default;

    explicit Socket(std::shared_ptr<base::Net::INativeSocket> nativeSocket);
    void reservedCreate();


    void write(base::Buffer data) override;

    bool getIsAnyError() override;
    bool getIsLogicError() override;
    bool getIsNetError() override;

    ArrayList<std::shared_ptr<base::StreamBuffer>>* getReceivedNetBuffersList() override;
    void lockReceiveNetBuffersList() override;
    void unlockReceiveNetBuffersList() override;

    void reservedDisposeMain() final;
    ~Socket() override;
protected:
    virtual void createMain();

    virtual void disposeMain();
private:
    void onNativeSocket_data(
        base::Net::INativeSocket* target,
        base::Buffer& data);
    void onNativeSocket_logicError(base::Net::INativeSocket* target);
    void onNativeSocket_netError(base::Net::INativeSocket* target);

    void disposeNativeSocket();

};
};
};
