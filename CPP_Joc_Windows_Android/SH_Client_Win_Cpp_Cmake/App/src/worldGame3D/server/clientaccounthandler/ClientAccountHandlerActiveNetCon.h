#pragma once

#include <base/net/tcp/GeneralTcpConnection.h>
#include <base/net/handler/CbListener_NetConHandler.h>
#include <base/httprequests/IHttpRequestsManager.h>
#include "ClientAccountHandlerNetConnector.h"

namespace worldGame3D {
class ClientAccountHandlerActiveNetCon : public ClientAccountHandlerNetConnector {
private: typedef ClientAccountHandlerNetConnector super;
private:
    class NetCodes_AccountsInteractor_Client {
    public:
        // ENCRYPTION: Server -> Client: AES(connectedDeviceKey)
        // ENCRYPTION: Client -> Server: AES(connectedDeviceKey + deviceTestResultString)

        static const int SC_C_SEND_ACCOUNT_STATUS = 100010;
        static const int CS_R_SEND_ACCOUNT_STATUS_NO_ACCOUNT = 100011;
        static const int CS_R_SEND_ACCOUNT_STATUS_OK_AND_DEVICE_CONNECTED = 100012;

        static const int SC_C_SET_NEW_CONNECTED_DEVICE_INFO = 100020;
        // connectedDeviceId: number;
        // connectedDeviceKey: string;
        static const int CS_R_SET_NEW_CONNECTED_DEVICE_INFO_OK = 100021;

        static const int SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT = 100030;
        // deviceTestString: string;
        static const int CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK = 100031;
        // connectedDeviceId: number;
        // eDeviceTestResultString: string; (length = md5.length); =MD5(connectedDeviceKey + deviceTestString)

        static const int SC_C_SET_SESSION_ID = 100040;
        // eSessionId: number;
        // eSessionKey: string;
        // eSocialUserId: number;
        // eNickname: string;
        static const int CS_OR_SET_SESSION_ID_OK = 100041;
        // This is a optional response. Can be ignored if the net con will be closed right after.

        static const int SC_C_ACCOUNT_INTERACTION_FAILED = 100050;
        static const int CS_OR_ACCOUNT_INTERACTION_FAILED_OK = 100051;

    };
private:
    static const bool IS_DEBUG = false;

    static const float IDLE_NET_CON_THRESHOLD_S;
    static const std::string NET_CRYPTO_ALGORITHM;
    static const int NET_CRYPTO_KEY_LENGTH_BYTES;

    base::Net::BasicNetConHandler* netConHandler = nullptr;

    base::Net::CbListener_NetConHandler netConListener{};

public:
    ClientAccountHandlerActiveNetCon(ClientAccountHandlerActiveNetCon const&) = delete;
    ClientAccountHandlerActiveNetCon(ClientAccountHandlerActiveNetCon &&) = default;
    ClientAccountHandlerActiveNetCon& operator=(ClientAccountHandlerActiveNetCon const&) = delete;
    ClientAccountHandlerActiveNetCon& operator=(ClientAccountHandlerActiveNetCon &&) = default;

    explicit ClientAccountHandlerActiveNetCon(ILogHandler* logger, IHandler_ClientAccountHandler* handler, base::Net::INativeSocketsHandler* nativeSocketsHandler, worldGame3D::IAppGame* app);

    ~ClientAccountHandlerActiveNetCon() override;
protected:
    void newConCreated(std::shared_ptr<base::Net::GeneralTcpConnection>& con);

    void disposeMain() override;
private:
    bool processMsg(base::Net::IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer);

    void sendMsg_CS_R_SEND_ACCOUNT_STATUS_NO_ACCOUNT(base::Net::IGeneralTcpCon& con);
    void sendMsg_CS_R_SEND_ACCOUNT_STATUS_OK_AND_DEVICE_CONNECTED(base::Net::IGeneralTcpCon& con);
    void sendMsg_CS_R_SET_NEW_CONNECTED_DEVICE_INFO_OK(base::Net::IGeneralTcpCon& con);
    void sendMsg_CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK(base::Net::IGeneralTcpCon& con, int connectedDeviceId, std::string& deviceTestResultString);
    void sendMsg_CS_OR_SET_SESSION_ID_OK(base::Net::IGeneralTcpCon& con);

    bool onNetConProcessMsg(
        base::Net::BasicNetConHandler* target,
        base::Net::IGeneralTcpCon& con,
        int msgLen, int msgType, base::StreamBuffer& msgNetBuffer);
    void onNetConNetError(base::Net::BasicNetConHandler* target);
    void onNetConLogicError(base::Net::BasicNetConHandler* target);

};
};
