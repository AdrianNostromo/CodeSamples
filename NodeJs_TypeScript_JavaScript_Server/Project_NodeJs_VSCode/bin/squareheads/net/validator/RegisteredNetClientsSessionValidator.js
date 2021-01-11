"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const NetClientsHolder_1 = require("base/common/client/net/holder/NetClientsHolder");
const CryptoUtil_1 = require("base/crypto/util/CryptoUtil");
const NetMsgsBlock_1 = require("base/common/net/NetMsgsBlock");
const UnvalidatedClient_1 = require("./util/UnvalidatedClient");
const md5 = require("md5");
const GlobalAccessLogger_1 = require("base/log/GlobalAccessLogger");
class NetCodes_Server_UnvalidatedClient {
}
// ENCRYPTION: Server -> Client: AES(sessionKey)
// ENCRYPTION: Client -> Server: AES(sessionKey + sessionTestResultString)
NetCodes_Server_UnvalidatedClient.SC_C_SEND_VALIDATION_DATA = 100010;
// sessionTestString: string;
NetCodes_Server_UnvalidatedClient.CS_R_SEND_VALIDATION_DATA_OK = 100011;
// connectedDeviceId: number;
// eSessionTestResultString: string; (length = md5.length); =md5(sessionKey + sessionTestString)
NetCodes_Server_UnvalidatedClient.SC_C_SET_SESSION_STATUS_VALID = 100020;
NetCodes_Server_UnvalidatedClient.CS_R_SET_SESSION_STATUS_VALID_OK = 100021;
NetCodes_Server_UnvalidatedClient.SC_C_CLIENT_VALIDATION_FAILED = 100030;
NetCodes_Server_UnvalidatedClient.CS_OR_CLIENT_VALIDATION_FAILED_OK = 100031;
class RegisteredNetClientsSessionValidator extends NetClientsHolder_1.NetClientsHolder {
    constructor(cbValidationSuccess, clientDevicesManager, clientSessionsManager) {
        super(null, null, null);
        this.cbValidationSuccess = cbValidationSuccess;
        this.clientDevicesManager = clientDevicesManager;
        this.clientSessionsManager = clientSessionsManager;
    }
    addClientCon(con) {
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('Client session validation. addClientCon.');
        let client = new UnvalidatedClient_1.UnvalidatedClient(con);
        super.addClient(client);
    }
    sendMsg_SC_C_SEND_VALIDATION_DATA(client, sessionTestString) {
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('Client session validation. sendMsg_SC_C_SEND_VALIDATION_DATA.');
        client.setCurrentCommandNetCode(NetCodes_Server_UnvalidatedClient.SC_C_SEND_VALIDATION_DATA);
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4
            + NetMsgsBlock_1.NetMsgsBlock.stringInBufferSize(sessionTestString))
            .startMsgWrite()
            .writeInt(NetCodes_Server_UnvalidatedClient.SC_C_SEND_VALIDATION_DATA)
            .writeString(sessionTestString)
            .endMsgWrite());
    }
    sendMsg_SC_C_SET_SESSION_STATUS_VALID(client) {
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('Client session validation. sendMsg_SC_C_SET_SESSION_STATUS_VALID.');
        client.setCurrentCommandNetCode(NetCodes_Server_UnvalidatedClient.SC_C_SET_SESSION_STATUS_VALID);
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4)
            .startMsgWrite()
            .writeInt(NetCodes_Server_UnvalidatedClient.SC_C_SET_SESSION_STATUS_VALID)
            .endMsgWrite());
    }
    sendMsg_SC_C_CLIENT_VALIDATION_FAILED(client) {
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('Client session validation. sendMsg_SC_C_CLIENT_VALIDATION_FAILED.');
        client.setCurrentCommandNetCode(NetCodes_Server_UnvalidatedClient.SC_C_CLIENT_VALIDATION_FAILED);
        client.getCon().sendBlock(new NetMsgsBlock_1.NetMsgsBlock(1, 4)
            .startMsgWrite()
            .writeInt(NetCodes_Server_UnvalidatedClient.SC_C_CLIENT_VALIDATION_FAILED)
            .endMsgWrite());
    }
    onNewClient(client) {
        super.onNewClient(client);
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('Client session validation. onNewClient.');
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('Starting player validation.');
        client.forceDisconnectTimeoutS = RegisteredNetClientsSessionValidator.MAX_VALIDATION_DURATION_S;
        client.sessionTestString = CryptoUtil_1.CryptoUtil.randomHexString(RegisteredNetClientsSessionValidator.SESSION_TEST_STRING_LENGTH);
        try {
            this.sendMsg_SC_C_SEND_VALIDATION_DATA(client, client.sessionTestString);
        }
        catch (err) {
            this.removeClient(client);
        }
    }
    onClientRemoved_pre(client) {
        super.onClientRemoved_pre(client);
        //void
    }
    onProcessMsg(client, msgLen, msgType, msgNetBuffer) {
        if (super.onProcessMsg(client, msgLen, msgType, msgNetBuffer)) {
            return true;
        }
        switch (msgType) {
            case NetCodes_Server_UnvalidatedClient.CS_R_SEND_VALIDATION_DATA_OK:
                this.processMsg_CS_R_SEND_VALIDATION_DATA_OK(client, msgLen, msgType, msgNetBuffer);
                break;
            case NetCodes_Server_UnvalidatedClient.CS_R_SET_SESSION_STATUS_VALID_OK:
                this.processMsg_CS_R_SET_SESSION_STATUS_VALID_OK(client, msgLen, msgType, msgNetBuffer);
                break;
            case NetCodes_Server_UnvalidatedClient.CS_OR_CLIENT_VALIDATION_FAILED_OK:
                this.processMsg_CS_OR_CLIENT_VALIDATION_FAILED_OK(client, msgLen, msgType, msgNetBuffer);
                break;
            default:
                return false;
        }
        return true;
    }
    processMsg_CS_R_SEND_VALIDATION_DATA_OK(client, msgLen, msgType, msgNetBuffer) {
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('Client session validation. processMsg_CS_R_SEND_VALIDATION_DATA_OK.');
        if (client.getCurrentCommandNetCode() != NetCodes_Server_UnvalidatedClient.SC_C_SEND_VALIDATION_DATA) {
            throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();
        let connectedDeviceId = msgNetBuffer.readInt();
        // Validate the client account.
        let serverConnectedDeviceInfo = this.clientDevicesManager.getConnectedDeviceInfoFromDeviceId(connectedDeviceId);
        if (serverConnectedDeviceInfo == null) {
            this.sendMsg_SC_C_CLIENT_VALIDATION_FAILED(client);
            client.forceDisconnectTimeoutS = RegisteredNetClientsSessionValidator.MAX_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            // Device not found on the server.
            GlobalAccessLogger_1.GlobalAccessLogger.getLog().securityWarning("Client tried to connect with a inexistent device id.");
            return;
        }
        let clientSessionInformation = this.clientSessionsManager.getOrCreateClientSessionWithAccId(serverConnectedDeviceInfo.accountId);
        if (clientSessionInformation == null) {
            this.sendMsg_SC_C_CLIENT_VALIDATION_FAILED(client);
            client.forceDisconnectTimeoutS = RegisteredNetClientsSessionValidator.MAX_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            // Device not found on the server.
            GlobalAccessLogger_1.GlobalAccessLogger.getLog().securityWarning("Client tried to connect with a inexistent device id.");
            return;
        }
        // Generate the on_server test result;
        let server_sessionTestResultString = md5(clientSessionInformation.key + client.sessionTestString);
        // Initialize encription.
        let encryptionKeyString = md5(server_sessionTestResultString);
        let keyStringBuffer = Buffer.from(encryptionKeyString, "utf8");
        let key = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 32);
        let iv = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 8);
        client.getCon().encryptionCipher = CryptoUtil_1.CryptoUtil.newEncryptCipher(CryptoUtil_1.CryptoUtil.Algorithms_Salsa20, key, iv);
        // Initialize encription.
        let decryptionKeyString = server_sessionTestResultString;
        keyStringBuffer = Buffer.from(decryptionKeyString, "utf8");
        key = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 32);
        iv = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 8);
        client.getCon().decryptionCipher = CryptoUtil_1.CryptoUtil.newDecryptCipher(CryptoUtil_1.CryptoUtil.Algorithms_Salsa20, key, iv);
        // Read and decrypt the client session test result.
        let eClientSessionTestResultBase64 = msgNetBuffer.readString();
        let clientSessionTestResultString = client.getCon().decryptionCipher.decryptBase64ToString(eClientSessionTestResultBase64);
        if (clientSessionTestResultString.length != RegisteredNetClientsSessionValidator.SESSION_TEST_STRING_LENGTH) {
            this.sendMsg_SC_C_CLIENT_VALIDATION_FAILED(client);
            client.forceDisconnectTimeoutS = RegisteredNetClientsSessionValidator.MAX_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            GlobalAccessLogger_1.GlobalAccessLogger.getLog().securityWarning("Client session test failed.");
            return;
        }
        // Check if the client result is corect.
        if (server_sessionTestResultString != clientSessionTestResultString) {
            this.sendMsg_SC_C_CLIENT_VALIDATION_FAILED(client);
            client.forceDisconnectTimeoutS = RegisteredNetClientsSessionValidator.MAX_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            GlobalAccessLogger_1.GlobalAccessLogger.getLog().securityWarning("Client session test failed.");
            return;
        }
        client.posValidationAccountId = clientSessionInformation.accountId;
        this.sendMsg_SC_C_SET_SESSION_STATUS_VALID(client);
    }
    processMsg_CS_R_SET_SESSION_STATUS_VALID_OK(client, msgLen, msgType, msgNetBuffer) {
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('Client session validation. processMsg_CS_R_SET_SESSION_STATUS_VALID_OK.');
        if (client.getCurrentCommandNetCode() != NetCodes_Server_UnvalidatedClient.SC_C_SET_SESSION_STATUS_VALID) {
            throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();
        // Close the connection.
        client.markForRemoval = true;
        // Extract the con and call the connection validation success.
        let con = this.extractClientCon(client);
        this.onValidationSuccess(client.posValidationAccountId, con);
    }
    onValidationSuccess(accountId, con) {
        if (this.cbValidationSuccess != null) {
            this.cbValidationSuccess(accountId, con);
        }
    }
    processMsg_CS_OR_CLIENT_VALIDATION_FAILED_OK(client, msgLen, msgType, msgNetBuffer) {
        GlobalAccessLogger_1.GlobalAccessLogger.getLog().debug('Client session validation. processMsg_CS_OR_CLIENT_VALIDATION_FAILED_OK.');
        if (client.getCurrentCommandNetCode() != NetCodes_Server_UnvalidatedClient.SC_C_CLIENT_VALIDATION_FAILED) {
            throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();
        // Close the connection.
        client.markForRemoval = true;
    }
}
RegisteredNetClientsSessionValidator.MAX_VALIDATION_DURATION_S = 15;
RegisteredNetClientsSessionValidator.MAX_POST_VALIDATION_DELAYED_CLOSE_DURATION_S = 5;
RegisteredNetClientsSessionValidator.SESSION_TEST_STRING_LENGTH = 32;
exports.RegisteredNetClientsSessionValidator = RegisteredNetClientsSessionValidator;
//# sourceMappingURL=RegisteredNetClientsSessionValidator.js.map