#include "CbListener_NetConHandler.h"

using namespace base::Net;

CbListener_NetConHandler::CbListener_NetConHandler() {
    //void
}

bool CbListener_NetConHandler::onProcessMsg(BasicNetConHandler* target, IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer) {
    if(cb_onProcessMsg) {
        return cb_onProcessMsg(target, con, msgLen, msgType, msgNetBuffer);
    }

    return false;
}

void CbListener_NetConHandler::onNetError(BasicNetConHandler* target) {
    if(cb_onNetError) {
        cb_onNetError(target);
    }
}

void CbListener_NetConHandler::onLogicError(BasicNetConHandler* target) {
    if(cb_onLogicError) {
        cb_onLogicError(target);
    }
}

CbListener_NetConHandler::~CbListener_NetConHandler() {
    //void
}
