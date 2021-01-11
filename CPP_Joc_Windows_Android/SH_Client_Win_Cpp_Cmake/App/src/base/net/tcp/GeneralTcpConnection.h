#pragma once

#include "TcpCryptoConnection.h"
#include "IGeneralTcpCon.h"

namespace base {
namespace Net {
class GeneralTcpConnection : public TcpCryptoConnection, virtual public IGeneralTcpCon {
private: typedef TcpCryptoConnection super;
public:
    GeneralTcpConnection(GeneralTcpConnection const&) = delete;
    GeneralTcpConnection(GeneralTcpConnection &&) = default;
    GeneralTcpConnection& operator=(GeneralTcpConnection const&) = delete;
    GeneralTcpConnection& operator=(GeneralTcpConnection &&) = default;

    explicit GeneralTcpConnection(std::shared_ptr<Socket>& socket);

    virtual ~GeneralTcpConnection();
};
};
};
