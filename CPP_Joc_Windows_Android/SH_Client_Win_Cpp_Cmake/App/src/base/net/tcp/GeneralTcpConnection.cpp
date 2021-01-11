#include "GeneralTcpConnection.h"

using namespace base::Net;

GeneralTcpConnection::GeneralTcpConnection(std::shared_ptr<Socket>& socket)
    : super(socket)
{
    //void
}

GeneralTcpConnection::~GeneralTcpConnection() {
    //void
}
