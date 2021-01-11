#pragma once

#include "IListener_NetConHandler.h"
#include <functional>

namespace base {
namespace Net {
class CbListener_NetConHandler : virtual public IListener_NetConHandler {
public:
    std::function<bool(
        BasicNetConHandler* target,
        IGeneralTcpCon& con,
        int msgLen, int msgType, StreamBuffer& msgNetBuffer)> cb_onProcessMsg;
    std::function<void(
        BasicNetConHandler* target)> cb_onNetError;
    std::function<void(
        BasicNetConHandler* target)> cb_onLogicError;

public:
    CbListener_NetConHandler(CbListener_NetConHandler const&) = delete;
    CbListener_NetConHandler(CbListener_NetConHandler &&) = default;
    CbListener_NetConHandler& operator=(CbListener_NetConHandler const&) = delete;
    CbListener_NetConHandler& operator=(CbListener_NetConHandler &&) = default;

    explicit CbListener_NetConHandler();

    bool onProcessMsg(BasicNetConHandler* target, IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer) override;

    void onNetError(BasicNetConHandler* target) override;
    void onLogicError(BasicNetConHandler* target) override;

    ~CbListener_NetConHandler() override;
};
};
};
