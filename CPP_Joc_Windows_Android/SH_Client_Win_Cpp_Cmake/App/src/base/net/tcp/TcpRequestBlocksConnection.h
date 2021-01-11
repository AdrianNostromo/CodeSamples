#pragma once

#include "TcpDataBlocksConnection.h"

namespace base {
namespace Net {
class TcpRequestBlocksConnection : public TcpDataBlocksConnection {
private: typedef TcpDataBlocksConnection super;
public:
    TcpRequestBlocksConnection(TcpRequestBlocksConnection const&) = delete;
    TcpRequestBlocksConnection(TcpRequestBlocksConnection &&) = default;
    TcpRequestBlocksConnection& operator=(TcpRequestBlocksConnection const&) = delete;
    TcpRequestBlocksConnection& operator=(TcpRequestBlocksConnection &&) = default;

    explicit TcpRequestBlocksConnection(std::shared_ptr<Socket>& socket);

    std::shared_ptr<base::StreamBuffer> popNextMsg() override;

    ~TcpRequestBlocksConnection() override;
private:
    std::shared_ptr<base::StreamBuffer> getNextMessageFromCurrentBlockBuffer(bool isMsgMandatory);

};
};
};
