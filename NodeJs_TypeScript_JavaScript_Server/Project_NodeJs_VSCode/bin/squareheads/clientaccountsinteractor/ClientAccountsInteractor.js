"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ClientAccountsInteractorBase_1 = require("./ClientAccountsInteractorBase");
const TcpServer_1 = require("base/common/net/TcpServer");
const NetClientsHolder_1 = require("base/common/client/net/holder/NetClientsHolder");
const GeneralTcpConnection_1 = require("base/common/net/tcp/connection/GeneralTcpConnection");
const IpPort_1 = require("base/net/util/IpPort");
const ClientAccountsNetClient_1 = require("./util/ClientAccountsNetClient");
const NetUtil_1 = require("base/common/net/util/NetUtil");
const CryptoUtil_1 = require("base/crypto/util/CryptoUtil");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const md5 = require("md5");
const LogicError_1 = require("base/exception/LogicError");
const GlobalAccessLogger_1 = require("base/log/GlobalAccessLogger");
class NetCodes_AccountsInteractor_Client {
}
// ENCRYPTION: Server -> Client: AES(connectedDeviceKey)
// ENCRYPTION: Client -> Server: AES(connectedDeviceKey + deviceTestResultString)
NetCodes_AccountsInteractor_Client.SC_C_SEND_ACCOUNT_STATUS = 100010;
NetCodes_AccountsInteractor_Client.CS_R_SEND_ACCOUNT_STATUS_NO_ACCOUNT = 100011;
NetCodes_AccountsInteractor_Client.CS_R_SEND_ACCOUNT_STATUS_OK_AND_DEVICE_CONNECTED = 100012;
NetCodes_AccountsInteractor_Client.SC_C_SET_NEW_CONNECTED_DEVICE_INFO = 100020;
// connectedDeviceId: number;
// connectedDeviceKey: string;
NetCodes_AccountsInteractor_Client.CS_R_SET_NEW_CONNECTED_DEVICE_INFO_OK = 100021;
NetCodes_AccountsInteractor_Client.SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT = 100030;
// deviceTestString: string;
NetCodes_AccountsInteractor_Client.CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK = 100031;
// connectedDeviceId: number;
// eDeviceTestResultString: string; (length = md5.length); =MD5(connectedDeviceKey + deviceTestString)
NetCodes_AccountsInteractor_Client.SC_C_SET_SESSION_ID = 100040;
// eSessionId: number;
// eSessionKey: string;
// eSocialUserId: number;
// eNickname: string;
NetCodes_AccountsInteractor_Client.CS_OR_SET_SESSION_ID_OK = 100041;
// This is a optional response. Can be ignored if the net con will be closed right after.
NetCodes_AccountsInteractor_Client.SC_C_ACCOUNT_INTERACTION_FAILED = 100050;
NetCodes_AccountsInteractor_Client.CS_OR_ACCOUNT_INTERACTION_FAILED_OK = 100051;
class ClientAccountsInteractor extends ClientAccountsInteractorBase_1.ClientAccountsInteractorBase {
    constructor(app) {
        super(app);
        this.netClientsListener = null;
        this.netClientsHolder = null;
        //void
    }
    create_framework() {
        super.create_framework();
        this.netClientsListener = new TcpServer_1.TcpServer(new IpPort_1.IpPort('127.0.0.1', 0), this.onNewSocket.bind(this), this.onListenerNetError.bind(this), this.onListenerLogicError.bind(this), null);
        this.netClientsListener.create();
        this.addComponent(this.netClientsHolder = new NetClientsHolder_1.NetClientsHolder(this.onNewClient.bind(this), this.onClientRemoved_pre.bind(this), this.onProcessClientMsg.bind(this)));
    }
    onNewSocket(target, socket) {
        let con = new GeneralTcpConnection_1.GeneralTcpConnection(socket);
        let netClient = new ClientAccountsNetClient_1.ClientAccountsNetClient(con);
        this.netClientsHolder.addClient(netClient);
    }
    onListenerNetError(target, error) {
        this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');
        this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
    }
    onListenerLogicError(target, error) {
        this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');
        this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
    }
    onNewClient(client) {
        client.forceDisconnectTimeoutS = ClientAccountsInteractor.MAX_ACCOUNT_HANDLING_DURATION_S;
        // Request the client account information.
        this.sendMsg_SC_C_SEND_ACCOUNT_STATUS(client);
        this.getApp().getLog().debug('Client validation. Connection occured. Starting validation process.');
    }
    onClientRemoved_pre(client) {
        //void
    }
    onProcessClientMsg(client, msgLen, msgType, msgNetBuffer) {
        switch (msgType) {
            case NetCodes_AccountsInteractor_Client.CS_R_SEND_ACCOUNT_STATUS_NO_ACCOUNT:
                if (client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_SEND_ACCOUNT_STATUS) {
                    throw new Error('Security warning.');
                }
                client.clearCurrentCommandNetCode();
                // Register a new account.
                let accountId = this.getApp().getSquareHeads().getClientAccountsManager().createNewSquareHeadsAccount();
                // Set a new connected device.
                let connectedDeviceInfo = this.getApp().getSquareHeads().getClientDevicesManager().connectNewDeviceToAccount(accountId);
                // Send the new connected device id.
                this.sendMsg_SC_C_SET_NEW_CONNECTED_DEVICE_INFO(client, connectedDeviceInfo.id, connectedDeviceInfo.key);
                this.getApp().getLog().debug('Client validation. New account created.');
                break;
            case NetCodes_AccountsInteractor_Client.CS_R_SEND_ACCOUNT_STATUS_OK_AND_DEVICE_CONNECTED:
                if (client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_SEND_ACCOUNT_STATUS) {
                    throw new Error('Security warning.');
                }
                client.clearCurrentCommandNetCode();
                // 32 random hex characters.
                client.deviceTestString = CryptoUtil_1.CryptoUtil.randomHexString(ClientAccountsInteractor.DEVICE_TEST_STRING_LENGTH);
                // Request the connected device id and test result.
                this.sendMsg_SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT(client, client.deviceTestString);
                this.getApp().getLog().debug('Client validation. Testing existent connected device.');
                break;
            case NetCodes_AccountsInteractor_Client.CS_R_SET_NEW_CONNECTED_DEVICE_INFO_OK:
                if (client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_SET_NEW_CONNECTED_DEVICE_INFO) {
                    throw new Error('Security warning.');
                }
                client.clearCurrentCommandNetCode();
                this.sendMsg_SC_C_SEND_ACCOUNT_STATUS(client);
                this.getApp().getLog().debug('Client validation. Account created. Restarting validation process.');
                break;
            case NetCodes_AccountsInteractor_Client.CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK:
                this.processMsg_CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK(client, msgLen, msgType, msgNetBuffer);
                break;
            case NetCodes_AccountsInteractor_Client.CS_OR_SET_SESSION_ID_OK:
                if (client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_SET_SESSION_ID) {
                    throw new Error('Security warning.');
                }
                client.clearCurrentCommandNetCode();
                // Close the connection.
                client.markForRemoval = true;
                break;
            case NetCodes_AccountsInteractor_Client.CS_OR_ACCOUNT_INTERACTION_FAILED_OK:
                if (client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_ACCOUNT_INTERACTION_FAILED) {
                    throw new Error('Security warning.');
                }
                client.clearCurrentCommandNetCode();
                // Close the connection.
                client.markForRemoval = true;
                break;
            default:
                return false;
        }
        return true;
    }
    processMsg_CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK(client, msgLen, msgType, msgNetBuffer) {
        if (client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT) {
            this.getApp().getLog().security("Client failed device validation.");
            throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();
        let connectedDeviceId = msgNetBuffer.readInt();
        // Check if there is a device with the specified id.
        // Also get the device key.
        let serverConnectedDeviceInfo = this.getApp().getSquareHeads().getClientDevicesManager().getConnectedDeviceInfoFromDeviceId(connectedDeviceId);
        if (serverConnectedDeviceInfo == null) {
            this.sendMsg_SC_C_ACCOUNT_INTERACTION_FAILED(client);
            client.forceDisconnectTimeoutS = ClientAccountsInteractor.MAX_ACCOUNT_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            // Device not found on the server.
            this.getApp().getLog().securityWarning("Client tried to connect with a inexistent device id.");
            return;
        }
        let serverDeviceTestResult = md5(serverConnectedDeviceInfo.key + client.deviceTestString);
        // Initialize encription.
        let decryptionKeyString = serverDeviceTestResult;
        let keyStringBuffer = Buffer.from(decryptionKeyString, "utf8");
        let key = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 32);
        let iv = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 8);
        client.getCon().decryptionCipher = CryptoUtil_1.CryptoUtil.newDecryptCipher(CryptoUtil_1.CryptoUtil.Algorithms_Salsa20, key, iv);
        let eClientDeviceTestResultBase64 = msgNetBuffer.readString();
        let clientDeviceTestResultString = client.getCon().decryptionCipher.decryptBase64ToString(eClientDeviceTestResultBase64);
        if (clientDeviceTestResultString.length != ClientAccountsInteractor.DEVICE_TEST_STRING_LENGTH) {
            this.sendMsg_SC_C_ACCOUNT_INTERACTION_FAILED(client);
            client.forceDisconnectTimeoutS = ClientAccountsInteractor.MAX_ACCOUNT_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            this.getApp().getLog().securityWarning("Client failed device test length.");
            return;
        }
        if (serverDeviceTestResult != clientDeviceTestResultString) {
            this.sendMsg_SC_C_ACCOUNT_INTERACTION_FAILED(client);
            client.forceDisconnectTimeoutS = ClientAccountsInteractor.MAX_ACCOUNT_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            this.getApp().getLog().securityWarning("Client failed device test.");
            return;
        }
        // Client passed device test. Get valid session information and  send it.
        // Initialize encription.
        let encryptionKeyString = md5(serverDeviceTestResult);
        keyStringBuffer = Buffer.from(encryptionKeyString, "utf8");
        key = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 32);
        iv = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 8);
        client.getCon().encryptionCipher = CryptoUtil_1.CryptoUtil.newEncryptCipher(CryptoUtil_1.CryptoUtil.Algorithms_Salsa20, key, iv);
        let accountId = serverConnectedDeviceInfo.accountId;
        // Get existent or create a new session id and key.
        let sessionInformation = this.getApp().getSquareHeads().getClientSessionsManager().getOrCreateClientSessionWithAccId(accountId);
        let userInfo_socialUserId_nickname = this.getApp().getSquareHeads().getClientAccountsManager().getUserCustomDataFromAccId_socialUserId_nickanme(accountId);
        // Response is valid. Send a new ession id.
        this.sendMsg_SC_C_SET_SESSION_ID(client, sessionInformation.id, sessionInformation.key, userInfo_socialUserId_nickname.socialUserId, userInfo_socialUserId_nickname.nickname);
        this.getApp().getLog().debug('Client validation. Ok. Sent client session.');
    }
    sendMsg_SC_C_SEND_ACCOUNT_STATUS(client) {
        client.setCurrentCommandNetCode(NetCodes_AccountsInteractor_Client.SC_C_SEND_ACCOUNT_STATUS);
        try {
            NetUtil_1.NetUtil.sendSimpleNetMsg(client.getCon(), NetCodes_AccountsInteractor_Client.SC_C_SEND_ACCOUNT_STATUS);
        }
        catch (err) {
            throw err;
        }
    }
    sendMsg_SC_C_SET_NEW_CONNECTED_DEVICE_INFO(client, connectedDeviceId, connectedDeviceKey) {
        client.setCurrentCommandNetCode(NetCodes_AccountsInteractor_Client.SC_C_SET_NEW_CONNECTED_DEVICE_INFO);
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4
            + 4
            + NetMsgsBlock_1.NetMsgsBlock.stringInBufferSize(connectedDeviceKey))
            .startMsgWrite()
            .writeInt(NetCodes_AccountsInteractor_Client.SC_C_SET_NEW_CONNECTED_DEVICE_INFO)
            .writeInt(connectedDeviceId)
            .writeString(connectedDeviceKey)
            .endMsgWrite());
    }
    sendMsg_SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT(client, deviceTestString) {
        client.setCurrentCommandNetCode(NetCodes_AccountsInteractor_Client.SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT);
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4
            + NetMsgsBlock_1.NetMsgsBlock.stringInBufferSize(deviceTestString))
            .startMsgWrite()
            .writeInt(NetCodes_AccountsInteractor_Client.SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT)
            .writeString(deviceTestString)
            .endMsgWrite());
    }
    sendMsg_SC_C_SET_SESSION_ID(client, sessionId, sessionKey, socialUserId, nickname) {
        if (client.getCon().encryptionCipher == null) {
            throw new LogicError_1.LogicError("ClientAccountsInteractor.sendMsg_SC_C_SET_SESSION_ID.1.");
        }
        let eSessionId = client.getCon().encryptionCipher.encryptIntToBuffer(sessionId);
        let eSessionKey = client.getCon().encryptionCipher.encryptStringToBase64(sessionKey);
        let eSocialUserId = client.getCon().encryptionCipher.encryptIntToBuffer(socialUserId);
        let eNickname = client.getCon().encryptionCipher.encryptStringToBase64(nickname);
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug("Client session generated. sessionKey: " + sessionKey + ", eSessionKey: " + eSessionKey + ", nickname: " + nickname + ", eNickname: " + eNickname + ", sessionId: " + sessionId + ", socialUserId: " + socialUserId);
        client.setCurrentCommandNetCode(NetCodes_AccountsInteractor_Client.SC_C_SET_SESSION_ID);
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4
            + eSessionId.length
            + NetMsgsBlock_1.NetMsgsBlock.stringInBufferSize(eSessionKey)
            + eSocialUserId.length
            + NetMsgsBlock_1.NetMsgsBlock.stringInBufferSize(eNickname))
            .startMsgWrite()
            .writeInt(NetCodes_AccountsInteractor_Client.SC_C_SET_SESSION_ID)
            .writeBuffer(eSessionId)
            .writeString(eSessionKey)
            .writeBuffer(eSocialUserId)
            .writeString(eNickname)
            .endMsgWrite());
        // Connection finished. start a timeout.
        client.forceDisconnectTimeoutS = ClientAccountsInteractor.MAX_ACCOUNT_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
    }
    sendMsg_SC_C_ACCOUNT_INTERACTION_FAILED(client) {
        client.setCurrentCommandNetCode(NetCodes_AccountsInteractor_Client.SC_C_ACCOUNT_INTERACTION_FAILED);
        try {
            NetUtil_1.NetUtil.sendSimpleNetMsg(client.getCon(), NetCodes_AccountsInteractor_Client.SC_C_ACCOUNT_INTERACTION_FAILED);
        }
        catch (err) {
            throw err;
        }
    }
    getClientsListenerIpPort() {
        if (this.netClientsListener != null) {
            let ret = this.netClientsListener.getIpPort();
            return ret;
        }
        return null;
    }
}
ClientAccountsInteractor.MAX_ACCOUNT_HANDLING_DURATION_S = 15;
ClientAccountsInteractor.MAX_ACCOUNT_POST_VALIDATION_DELAYED_CLOSE_DURATION_S = 5;
ClientAccountsInteractor.DEVICE_TEST_STRING_LENGTH = 32;
exports.ClientAccountsInteractor = ClientAccountsInteractor;
//# sourceMappingURL=ClientAccountsInteractor.js.map