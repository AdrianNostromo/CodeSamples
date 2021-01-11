#pragma once

#include "BasicTcpConnection.h"
#include "IDataBlocksTcpCon.h"

namespace base {
namespace Net {
class TcpDataBlocksConnection : public BasicTcpConnection, virtual public IDataBlocksTcpCon {
private: typedef BasicTcpConnection super;
protected:
    std::shared_ptr<base::StreamBuffer> currentBlockNetBuffer = nullptr;

private:
    int lastReceivedDataBlockIndice = 0;
    int sendDataBlockIndiceCounter = 0;

public:
    TcpDataBlocksConnection(TcpDataBlocksConnection const&) = delete;
    TcpDataBlocksConnection(TcpDataBlocksConnection &&) = default;
    TcpDataBlocksConnection& operator=(TcpDataBlocksConnection const&) = delete;
    TcpDataBlocksConnection& operator=(TcpDataBlocksConnection &&) = default;

    explicit TcpDataBlocksConnection(std::shared_ptr<Socket>& socket);

    void sendBlock(std::shared_ptr<NetMsgsBlock> blockDataNetBuffer) override;

    ~TcpDataBlocksConnection() override;
protected:
    void handleBlockReceive();
    virtual std::shared_ptr<base::StreamBuffer> extractConfirmedReceivedBlock(int blockLen);

};
};
};
