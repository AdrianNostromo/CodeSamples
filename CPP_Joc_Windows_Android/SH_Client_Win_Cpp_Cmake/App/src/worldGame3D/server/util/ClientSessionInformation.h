#pragma once

#include <string>
#include <memory>

namespace worldGame3D {
class ClientSessionInformation {
public:
    int sessionId;
    std::shared_ptr<std::string> sessionKey;

    int socialUserId;
    std::string nickname;

public:
    ClientSessionInformation(ClientSessionInformation const&) = delete;
    ClientSessionInformation(ClientSessionInformation &&) = default;
    ClientSessionInformation& operator=(ClientSessionInformation const&) = delete;
    ClientSessionInformation& operator=(ClientSessionInformation &&) = default;

    explicit ClientSessionInformation(int sessionId, std::shared_ptr<std::string> sessionKey, int socialUserId, std::string& nickname);

    virtual ~ClientSessionInformation();
};
};
