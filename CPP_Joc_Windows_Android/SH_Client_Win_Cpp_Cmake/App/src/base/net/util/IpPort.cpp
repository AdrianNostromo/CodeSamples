#include "IpPort.h"

using namespace base::Net;

IpPort::IpPort(std::string ip, int port)
    : ip(ip), port(port)
{
    //void
}

IpPort::IpPort() {
    //void
}

void IpPort::set(std::string& ip, int port) {
    this->ip = ip;
    this->port = port;
}

IpPort::~IpPort() {
    //void
}
