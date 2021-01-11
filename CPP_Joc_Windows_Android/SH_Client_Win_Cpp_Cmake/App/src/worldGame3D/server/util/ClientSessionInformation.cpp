#include "ClientSessionInformation.h"

using namespace worldGame3D;

ClientSessionInformation::ClientSessionInformation(int sessionId, std::shared_ptr<std::string> sessionKey, int socialUserId, std::string& nickname)
    : sessionId(sessionId), sessionKey(sessionKey), socialUserId(socialUserId), nickname(nickname)
{
    //void
}

ClientSessionInformation::~ClientSessionInformation() {
    //void
}
