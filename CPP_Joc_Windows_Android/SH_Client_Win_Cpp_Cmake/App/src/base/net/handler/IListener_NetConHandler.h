#pragma once

#include <base/buffer/StreamBuffer.h>

namespace base {
namespace Net {
class BasicNetConHandler;
class IGeneralTcpCon;

class IListener_NetConHandler {
public:
    virtual bool onProcessMsg(BasicNetConHandler* target, IGeneralTcpCon& con, int msgLen, int msgType, StreamBuffer& msgNetBuffer) = 0;

    virtual void onNetError(BasicNetConHandler* target) = 0;
    virtual void onLogicError(BasicNetConHandler* target) = 0;

    virtual ~IListener_NetConHandler() = default;
};
};
};
