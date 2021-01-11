#include <base/app/config/AppConfig.h>
#include <base/net/handler/BasicNetConHandler.h>
#include "ClientAccountHandlerActiveNetCon.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/server/util/ConnectedDeviceInformation.h>
#include <base/hash/md5/MD5.h>
#include <base/crypto/util/CryptoUtil.h>
#include <base/buffer/util/BitsUtil.h>
#include <assert.h>
#include <worldGame3D/app/IAppGame.h>

using namespace worldGame3D;

const float ClientAccountHandlerActiveNetCon::IDLE_NET_CON_THRESHOLD_S = 3.0f;

const std::string ClientAccountHandlerActiveNetCon::NET_CRYPTO_ALGORITHM = "_salsa20";
const int ClientAccountHandlerActiveNetCon::NET_CRYPTO_KEY_LENGTH_BYTES = 128 / 8;

ClientAccountHandlerActiveNetCon::ClientAccountHandlerActiveNetCon(ILogHandler* logger, IHandler_ClientAccountHandler* handler, base::Net::INativeSocketsHandler* nativeSocketsHandler, worldGame3D::IAppGame* app)
    : super(logger, handler, nativeSocketsHandler, app)
{
    netConListener.cb_onProcessMsg = std::bind(&ClientAccountHandlerActiveNetCon::onNetConProcessMsg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
    netConListener.cb_onNetError = std::bind(&ClientAccountHandlerActiveNetCon::onNetConNetError, this, std::placeholders::_1);
    netConListener.cb_onLogicError = std::bind(&ClientAccountHandlerActiveNetCon::onNetConLogicError, this, std::placeholders::_1);
}

void ClientAccountHandlerActiveNetCon::newConCreated(std::shared_ptr<base::Net::GeneralTcpConnection>& con) {
    addComponent(netConHandler = new base::Net::BasicNetConHandler(
        app,
        con,
        IDLE_NET_CON_THRESHOLD_S,
        &netConListener
    ), true/*doCreate*/);
}

bool ClientAccountHandlerActiveNetCon::processMsg(base::Net::IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer) {
    switch (msgType) {
        case NetCodes_AccountsInteractor_Client::SC_C_SEND_ACCOUNT_STATUS: {
            if (AppConfig::IS_DEBUG && IS_DEBUG) {
                logger->postLog("DevLog. Account. SC_C_SEND_ACCOUNT_STATUS.");
            }

            if (handler->getDeviceInformation() != nullptr) {
                sendMsg_CS_R_SEND_ACCOUNT_STATUS_OK_AND_DEVICE_CONNECTED(con);
            } else {
                sendMsg_CS_R_SEND_ACCOUNT_STATUS_NO_ACCOUNT(con);
            }

            break;
        }
        case NetCodes_AccountsInteractor_Client::SC_C_SET_NEW_CONNECTED_DEVICE_INFO: {
            if(AppConfig::IS_DEBUG && IS_DEBUG) {
                logger->postLog("DevLog. Account. SC_C_SET_NEW_CONNECTED_DEVICE_INFO.");
            }

            int connectedDeviceId = msgNetBuffer.readInt();
            std::string s = msgNetBuffer.readString();
            std::shared_ptr<std::string> connectedDeviceKey = std::make_shared<std::string>(s);

            if(handler != nullptr) {
                handler->onDeviceConnect_ok(this, connectedDeviceId, connectedDeviceKey);
            }

            sendMsg_CS_R_SET_NEW_CONNECTED_DEVICE_INFO_OK(con);

            break;
        }
        case NetCodes_AccountsInteractor_Client::SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT:{
            if(AppConfig::IS_DEBUG && IS_DEBUG) {
                logger->postLog("DevLog. Account. SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT.");
            }

            std::string deviceTestString = msgNetBuffer.readString();

            ConnectedDeviceInformation* connectedDeviceInformation = handler->getDeviceInformation();
            if(connectedDeviceInformation == nullptr) {
                throw LogicException(LOC);
            }

            int deviceId = connectedDeviceInformation->deviceId;
            std::string deviceTestResultString = base::hash::md5(*connectedDeviceInformation->deviceKey + deviceTestString);

            std::string encryptionKeyString = deviceTestResultString;
            con.setEncryptionCipher(base::crypto::CryptoUtil::newEncryptCipher(
                NET_CRYPTO_ALGORITHM,
                encryptionKeyString.data(), static_cast<int>(encryptionKeyString.length()), encryptionKeyString.data(), 8
            ));

            std::string decryptionKeyString = base::hash::md5(deviceTestResultString);
            con.setDecryptionCipher(base::crypto::CryptoUtil::newDecryptCipher(
                NET_CRYPTO_ALGORITHM,
                decryptionKeyString.data(), static_cast<int>(decryptionKeyString.length()), decryptionKeyString.data(), 8
            ));

            sendMsg_CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK(con, deviceId, deviceTestResultString);

            break;
        }
        case NetCodes_AccountsInteractor_Client::SC_C_SET_SESSION_ID:{
            if(AppConfig::IS_DEBUG && IS_DEBUG) {
                logger->postLog("DevLog. Account. SC_C_SET_SESSION_ID.");
            }

            if(con.getDecryptionCipher() == nullptr) {
                throw LogicException(LOC);
            }

            base::Buffer eSessionId = msgNetBuffer.readBuffer(4);
            std::string eSessionKey = msgNetBuffer.readString();
            base::Buffer eSocialUserId = msgNetBuffer.readBuffer(4);
            std::string eNickname = msgNetBuffer.readString();

            int sessionId = con.getDecryptionCipher()->decryptBytesToInt(eSessionId.getBlockCharPointer());
            std::string sessionKey = con.getDecryptionCipher()->decryptBase64ToString(eSessionKey);
            int socialUserId = con.getDecryptionCipher()->decryptBytesToInt(eSocialUserId.getBlockCharPointer());
            std::string nickname = con.getDecryptionCipher()->decryptBase64ToString(eNickname);

            std::shared_ptr<std::string> spSessionKey = std::make_shared<std::string>(sessionKey);

            //sendMsg_CS_OR_SET_SESSION_ID_OK(con);

            handler->onSessionConnect_ok(
                this,
                sessionId,
                spSessionKey,
                socialUserId,
                nickname
            );

            break;
        }
        case NetCodes_AccountsInteractor_Client::SC_C_ACCOUNT_INTERACTION_FAILED:{
            if(AppConfig::IS_DEBUG && IS_DEBUG) {
                logger->postLog("DevLog. Account. SC_C_ACCOUNT_INTERACTION_FAILED.");
            }

            //sendMsg_CS_OR_SET_SESSION_ID_OK(con);

            handler->onAccountInteraction_failed(
                this
            );

            break;
        }
        default: {
            return false;
        }
    }

    return true;
}

void ClientAccountHandlerActiveNetCon::sendMsg_CS_R_SEND_ACCOUNT_STATUS_NO_ACCOUNT(base::Net::IGeneralTcpCon& con) {
    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        logger->postLog("DevLog. Account. sendMsg_CS_R_SEND_ACCOUNT_STATUS_NO_ACCOUNT.");
    }

    std::shared_ptr<base::Net::NetMsgsBlock> netBlock = std::make_shared<base::Net::NetMsgsBlock>(
        1, // msgsCount
        4 // msgDataLen
    );
    netBlock->startMsgWrite();
    netBlock->writeInt(NetCodes_AccountsInteractor_Client::CS_R_SEND_ACCOUNT_STATUS_NO_ACCOUNT);
    netBlock->endMsgWrite();
    con.sendBlock(netBlock);
}

void ClientAccountHandlerActiveNetCon::sendMsg_CS_R_SEND_ACCOUNT_STATUS_OK_AND_DEVICE_CONNECTED(base::Net::IGeneralTcpCon& con) {
    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        logger->postLog("DevLog. Account. sendMsg_CS_R_SEND_ACCOUNT_STATUS_OK_AND_DEVICE_CONNECTED.");
    }

    std::shared_ptr<base::Net::NetMsgsBlock> netBlock = std::make_shared<base::Net::NetMsgsBlock>(
        1, // msgsCount
        4 // msgDataLen
    );
    netBlock->startMsgWrite();
    netBlock->writeInt(NetCodes_AccountsInteractor_Client::CS_R_SEND_ACCOUNT_STATUS_OK_AND_DEVICE_CONNECTED);
    netBlock->endMsgWrite();
    con.sendBlock(netBlock);
}

void ClientAccountHandlerActiveNetCon::sendMsg_CS_R_SET_NEW_CONNECTED_DEVICE_INFO_OK(base::Net::IGeneralTcpCon& con) {
    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        logger->postLog("DevLog. Account. sendMsg_CS_R_SET_NEW_CONNECTED_DEVICE_INFO_OK.");
    }

    std::shared_ptr<base::Net::NetMsgsBlock> netBlock = std::make_shared<base::Net::NetMsgsBlock>(
        1, // msgsCount
        4 // msgDataLen
    );
    netBlock->startMsgWrite();
    netBlock->writeInt(NetCodes_AccountsInteractor_Client::CS_R_SET_NEW_CONNECTED_DEVICE_INFO_OK);
    netBlock->endMsgWrite();
    con.sendBlock(netBlock);
}

void ClientAccountHandlerActiveNetCon::sendMsg_CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK(base::Net::IGeneralTcpCon& con, int connectedDeviceId, std::string& deviceTestResultString) {
    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        logger->postLog("DevLog. Account. sendMsg_CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK.");
    }

    if(con.getEncryptionCipher() == nullptr) {
        throw LogicException(LOC);
    }

    std::string eDeviceTestResultString = con.getEncryptionCipher()->encryptStringToBase64(deviceTestResultString);

    std::shared_ptr<base::Net::NetMsgsBlock> netBlock = std::make_shared<base::Net::NetMsgsBlock>(
        1, // msgsCount
        4
        + 4
        + base::Net::NetMsgsBlock::stringInBufferSize(eDeviceTestResultString) // msgDataLen
    );
    netBlock->startMsgWrite();
    netBlock->writeInt(NetCodes_AccountsInteractor_Client::CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK);
    netBlock->writeInt(connectedDeviceId);
    netBlock->writeString(eDeviceTestResultString);
    netBlock->endMsgWrite();
    con.sendBlock(netBlock);
}

void ClientAccountHandlerActiveNetCon::sendMsg_CS_OR_SET_SESSION_ID_OK(base::Net::IGeneralTcpCon& con) {
    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        logger->postLog("DevLog. Account. sendMsg_CS_OR_SET_SESSION_ID_OK.");
    }

    std::shared_ptr<base::Net::NetMsgsBlock> netBlock = std::make_shared<base::Net::NetMsgsBlock>(
        1, // msgsCount
        4 // msgDataLen
    );
    netBlock->startMsgWrite();
    netBlock->writeInt(NetCodes_AccountsInteractor_Client::CS_OR_SET_SESSION_ID_OK);
    netBlock->endMsgWrite();
    con.sendBlock(netBlock);
}

bool ClientAccountHandlerActiveNetCon::onNetConProcessMsg(
    base::Net::BasicNetConHandler* target,
    base::Net::IGeneralTcpCon& con,
    int msgLen, int msgType, base::StreamBuffer& msgNetBuffer)
{
    return processMsg(con, msgLen, msgType, msgNetBuffer);
}

void ClientAccountHandlerActiveNetCon::onNetConNetError(base::Net::BasicNetConHandler* target) {
    handler->onAccountHandling_netError(this);
}

void ClientAccountHandlerActiveNetCon::onNetConLogicError(base::Net::BasicNetConHandler* target) {
    handler->onAccountHandling_logicError(this);
}

void ClientAccountHandlerActiveNetCon::disposeMain() {
    if (netConHandler != nullptr) {
        removeComponent(netConHandler, true/*doDispose*/);

        netConHandler = nullptr;
    }

    super::disposeMain();
}

ClientAccountHandlerActiveNetCon::~ClientAccountHandlerActiveNetCon() {
    //void
}
