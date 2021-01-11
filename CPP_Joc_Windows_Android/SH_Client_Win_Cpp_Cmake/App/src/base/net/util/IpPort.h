#pragma once

#include <string>

namespace base {
namespace Net {
class IpPort {
public:
    std::string ip;
    int port;

public:
    IpPort(IpPort const&) = delete;
    IpPort(IpPort &&) = default;
    IpPort& operator=(IpPort const&) = delete;
    IpPort& operator=(IpPort &&) = default;

    explicit IpPort(std::string ip, int port);
    explicit IpPort();

    void set(std::string& ip, int port);

    virtual ~IpPort();
};
};
};
