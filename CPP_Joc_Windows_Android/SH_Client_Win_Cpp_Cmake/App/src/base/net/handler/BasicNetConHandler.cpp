#include <base/net/exception/NetConException.h>
#include <base/exceptions/DataFormatException.h>
#include <base/log/ILogHandler.h>
#include "BasicNetConHandler.h"
#include <base/exceptions/LogicException.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/IApp.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>

using namespace base::Net;

BasicNetConHandler::BasicNetConHandler(IApp* app, std::shared_ptr<GeneralTcpConnection> con, float idleThresholdS, IListener_NetConHandler* listener)
    : super(),
    app(app),
    con(con), idleThresholdS(idleThresholdS), listener(listener)
{
    con->setCb_onLogicError(std::bind(&BasicNetConHandler::onNetCon_logicError, this, std::placeholders::_1));
    con->setCb_onNetError(std::bind(&BasicNetConHandler::onNetCon_netError, this, std::placeholders::_1));
}

void BasicNetConHandler::create() {
    super::create();

    app->getLoopDistribution()->addEventListener(
        base::ILoopDistribution::AppLoopEvent_inputDispatch->type,
        std::bind(&BasicNetConHandler::onLoopEvent_appLoop_general, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );
}

std::shared_ptr<GeneralTcpConnection> BasicNetConHandler::extractNetCon() {
    if(con == nullptr) {
        throw LogicException(LOC);
    }

    std::shared_ptr<GeneralTcpConnection> extractedCon = con;
    con->clearCb_onLogicError();
    con->clearCb_onNetError();
    con = nullptr;

    return extractedCon;
}

std::shared_ptr<GeneralTcpConnection> BasicNetConHandler::getNetCon() {
    return con;
}

void BasicNetConHandler::createPost() {
    super::createPost();

    //void
}

void BasicNetConHandler::onNetCon_logicError(BasicTcpConnection* target) {
    listener->onLogicError(this);
}

void BasicNetConHandler::onNetCon_netError(BasicTcpConnection* target) {
    listener->onNetError(this);
}

void BasicNetConHandler::onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) {
    if(con != nullptr) {
        if (con->isData()) {
            idleTimeS = 0.0f;

            this->processConData(*con);
        }else {
            idleTimeS += event.deltaS;

            if(idleTimeS >= idleThresholdS) {
                // The con is x_dispose automatically.

                if(listener != nullptr) {
                    listener->onNetError(this);
                }
            }
        }
    }
}

void BasicNetConHandler::processConData(IGeneralTcpCon& con) {
    while(true) {
        std::shared_ptr<base::StreamBuffer> msgNetBuffer;
        int msgLen;
        int msgType;

        try {
            msgNetBuffer = con.popNextMsg();
        }catch(DataFormatException& x) {
            if(listener != nullptr) {
                listener->onNetError(this);
            }

            return ;
        }catch(Exception& x) {
            if(listener != nullptr) {
                listener->onNetError(this);
            }

            return ;
        }
        if(msgNetBuffer == nullptr) {
            return ;
        }

        try {
            msgLen = msgNetBuffer->readInt();
            msgType = msgNetBuffer->readInt();
        }catch(Exception& x) {
            if(listener != nullptr) {
                listener->onNetError(this);
            }

            return ;
        }

        bool isMsgHandled;
        try {
            isMsgHandled = onProcessMsg(con, msgLen, msgType, *msgNetBuffer);
        } catch (Exception& x) {
            if (listener != nullptr) {
                listener->onNetError(this);
            }

            return;
        }
        if (!isMsgHandled) {
            if (listener != nullptr) {
                listener->onNetError(this);
            }

            return;
        }
    }
}

bool BasicNetConHandler::onProcessMsg(IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer) {
    if(processMsg_local(con, msgLen, msgType, msgNetBuffer)) {
        return true;
    }

    if(listener != nullptr) {
        return listener->onProcessMsg(this, con, msgLen, msgType, msgNetBuffer);
    }

    return false;
}

bool BasicNetConHandler::processMsg_local(IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer) {
    switch (msgType) {
        case NetCodes::SC_C_PING:
            GlobalAccessLogHandler::devLogHandler->postDebug("Debug ping.");
            processMsg_SC_C_PING(con, msgLen, msgType, msgNetBuffer);

            break;
        default:
            return false;
    }

    return true;
}

void BasicNetConHandler::processMsg_SC_C_PING(IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer) {
    sendMsg_CS_R_PING_OK(con);
}

void BasicNetConHandler::sendMsg_CS_R_PING_OK(IGeneralTcpCon& con) {
    std::shared_ptr<base::Net::NetMsgsBlock> netBlock = std::make_shared<base::Net::NetMsgsBlock>(
        1, // msgsCount
        4 // msgDataLen
    );
    netBlock->startMsgWrite();
    netBlock->writeInt(NetCodes::CS_R_PING_OK);
    netBlock->endMsgWrite();
    con.sendBlock(netBlock);
}

void BasicNetConHandler::disposeMain() {
    if(listener != nullptr) {
        listener = nullptr;
    }

    if (con != nullptr) {
        con->reservedDisposeMain();

        con = nullptr;
    }

    super::disposeMain();
}

BasicNetConHandler::~BasicNetConHandler() {
    //void
}
