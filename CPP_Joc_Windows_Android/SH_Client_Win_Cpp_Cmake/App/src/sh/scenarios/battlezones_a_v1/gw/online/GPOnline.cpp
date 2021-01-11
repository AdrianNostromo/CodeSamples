#include <base/app/config/AppConfig.h>
#include <base/log/ILogHandler.h>
#include "GPOnline.h"

using namespace battlezones_a_v1;

GPOnline::GPOnline(base::IGameWorld* gw)
    : super(gw)
{
    //void
};

bool GPOnline::processMsg(base::Net::IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer) {
    switch (msgType) {
        case NetCodes::SC_C_SetSelfStatus: {
            if (AppConfig::IS_DEBUG && IS_DEBUG) {
                //GlobalAccessLogHandler::devLogHandler->postLog("DevLog. Account. SC_C_SetSelfStatus.");
            }

            sendMsg_CS_R_SetSelfStatus_OK(con);

            break;
        }
        default: {
            super::processMsg(con, msgLen, msgType, msgNetBuffer);
        }
    }

    return true;
}

void GPOnline::sendMsg_CS_R_SetSelfStatus_OK(base::Net::IGeneralTcpCon& con) {
    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        //GlobalAccessLogHandler::devLogHandler->postLog("DevLog. sendMsg_CS_R_SetSelfStatus_OK.");
    }

    std::shared_ptr<base::Net::NetMsgsBlock> netBlock = std::make_shared<base::Net::NetMsgsBlock>(
        1, // msgsCount
        4 // msgDataLen
    );
    netBlock->startMsgWrite();
    netBlock->writeInt(NetCodes::CS_R_SetSelfStatus_OK);
    netBlock->endMsgWrite();
    con.sendBlock(netBlock);
}

GPOnline::~GPOnline() {
    //void
};
