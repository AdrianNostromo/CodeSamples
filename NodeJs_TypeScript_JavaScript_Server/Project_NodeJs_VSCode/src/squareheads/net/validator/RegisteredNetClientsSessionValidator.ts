import { NetClientsHolder } from "base/common/client/net/holder/NetClientsHolder";

import { StreamBuffer } from "base/buffer/StreamBuffer";
import { CryptoUtil } from "base/crypto/util/CryptoUtil";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { UnvalidatedClient } from "./util/UnvalidatedClient";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ConnectedClientDeviceInfo } from "squareheads/clientdevicesmanager/util/ConnectedClientDeviceInfo";
import { ClientSessionInfo } from "squareheads/clientsessionsmanager/util/ClientSessionInfo";
import * as md5 from "md5";
import { ClientDevicesManager } from "squareheads/clientdevicesmanager/ClientDevicesManager";
import { ClientSessionsManager } from "squareheads/clientsessionsmanager/ClientSessionsManager";
import { GlobalAccessLogger } from "base/log/GlobalAccessLogger";

class NetCodes_Server_UnvalidatedClient {

  // ENCRYPTION: Server -> Client: AES(sessionKey)
  // ENCRYPTION: Client -> Server: AES(sessionKey + sessionTestResultString)

  public static readonly SC_C_SEND_VALIDATION_DATA: number = 100010;
  // sessionTestString: string;
  public static readonly CS_R_SEND_VALIDATION_DATA_OK: number = 100011;
  // connectedDeviceId: number;
  // eSessionTestResultString: string; (length = md5.length); =md5(sessionKey + sessionTestString)

  public static readonly SC_C_SET_SESSION_STATUS_VALID: number = 100020;
  public static readonly CS_R_SET_SESSION_STATUS_VALID_OK: number = 100021;
  
  public static readonly SC_C_CLIENT_VALIDATION_FAILED: number = 100030;
  public static readonly CS_OR_CLIENT_VALIDATION_FAILED_OK: number = 100031;

}

export class RegisteredNetClientsSessionValidator extends NetClientsHolder<UnvalidatedClient> {

  private static MAX_VALIDATION_DURATION_S: number = 15;
  private static MAX_POST_VALIDATION_DELAYED_CLOSE_DURATION_S: number = 5;
  
  private static SESSION_TEST_STRING_LENGTH: number = 32;
  
  /**
  * @return
  * true : valid request
  * false : not valid
  **/
  private cbValidationSuccess: (accountId: number, con: GeneralTcpConnection) => void;
  private clientDevicesManager: ClientDevicesManager;
  private clientSessionsManager: ClientSessionsManager;

  public constructor(
    cbValidationSuccess: (accountId: number, con: GeneralTcpConnection) => void,
    clientDevicesManager: ClientDevicesManager,
    clientSessionsManager: ClientSessionsManager
  ) {
    super(null, null, null);

    this.cbValidationSuccess = cbValidationSuccess;
    this.clientDevicesManager = clientDevicesManager;
    this.clientSessionsManager = clientSessionsManager;
  }

  public addClientCon(con: GeneralTcpConnection): void {
    GlobalAccessLogger.getLog().debug('Client session validation. addClientCon.');

    let client: UnvalidatedClient = new UnvalidatedClient(con);

    super.addClient(client);
  }
  
  private sendMsg_SC_C_SEND_VALIDATION_DATA(client: UnvalidatedClient, sessionTestString: string): void {
    GlobalAccessLogger.getLog().debug('Client session validation. sendMsg_SC_C_SEND_VALIDATION_DATA.');

    client.setCurrentCommandNetCode(NetCodes_Server_UnvalidatedClient.SC_C_SEND_VALIDATION_DATA);
    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4
      + NetMsgsBlock.stringInBufferSize(sessionTestString))
      .startMsgWrite()
      .writeInt(NetCodes_Server_UnvalidatedClient.SC_C_SEND_VALIDATION_DATA)
      .writeString(sessionTestString)
      .endMsgWrite()
    );
  }
  
  private sendMsg_SC_C_SET_SESSION_STATUS_VALID(client: UnvalidatedClient): void {
    GlobalAccessLogger.getLog().debug('Client session validation. sendMsg_SC_C_SET_SESSION_STATUS_VALID.');

    client.setCurrentCommandNetCode(NetCodes_Server_UnvalidatedClient.SC_C_SET_SESSION_STATUS_VALID);
    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4)
      .startMsgWrite()
      .writeInt(NetCodes_Server_UnvalidatedClient.SC_C_SET_SESSION_STATUS_VALID)
      .endMsgWrite()
    );
  }
  
  private sendMsg_SC_C_CLIENT_VALIDATION_FAILED(client: UnvalidatedClient): void {
    GlobalAccessLogger.getLog().debug('Client session validation. sendMsg_SC_C_CLIENT_VALIDATION_FAILED.');

    client.setCurrentCommandNetCode(NetCodes_Server_UnvalidatedClient.SC_C_CLIENT_VALIDATION_FAILED);
    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4)
      .startMsgWrite()
      .writeInt(NetCodes_Server_UnvalidatedClient.SC_C_CLIENT_VALIDATION_FAILED)
      .endMsgWrite()
    );
  }

  protected onNewClient(client: UnvalidatedClient): void {
    super.onNewClient(client);

    GlobalAccessLogger.getLog().debug('Client session validation. onNewClient.');

    GlobalAccessLogger.getLog().debug('Starting player validation.');

    client.forceDisconnectTimeoutS = RegisteredNetClientsSessionValidator.MAX_VALIDATION_DURATION_S;

    client.sessionTestString = CryptoUtil.randomHexString(RegisteredNetClientsSessionValidator.SESSION_TEST_STRING_LENGTH);
    
    try {
      this.sendMsg_SC_C_SEND_VALIDATION_DATA(
        client,
        client.sessionTestString
      );
    } catch (err) {
      this.removeClient(client);
    }
  }

  protected onClientRemoved_pre(client: UnvalidatedClient): void {
    super.onClientRemoved_pre(client);

    //void
  }

  protected onProcessMsg(client: UnvalidatedClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(super.onProcessMsg(client, msgLen, msgType, msgNetBuffer)) {
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

    private processMsg_CS_R_SEND_VALIDATION_DATA_OK(client: UnvalidatedClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): void {
        GlobalAccessLogger.getLog().debug('Client session validation. processMsg_CS_R_SEND_VALIDATION_DATA_OK.');

        if(client.getCurrentCommandNetCode() != NetCodes_Server_UnvalidatedClient.SC_C_SEND_VALIDATION_DATA) {
            throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();

        let connectedDeviceId: number = msgNetBuffer.readInt();

        // Validate the client account.
        let serverConnectedDeviceInfo: ConnectedClientDeviceInfo = this.clientDevicesManager.getConnectedDeviceInfoFromDeviceId(connectedDeviceId);
        if(serverConnectedDeviceInfo == null) {
            this.sendMsg_SC_C_CLIENT_VALIDATION_FAILED(client);
            client.forceDisconnectTimeoutS = RegisteredNetClientsSessionValidator.MAX_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            
            // Device not found on the server.
            GlobalAccessLogger.getLog().securityWarning("Client tried to connect with a inexistent device id.");

            return ;
        }

        let clientSessionInformation: ClientSessionInfo = this.clientSessionsManager.getOrCreateClientSessionWithAccId(serverConnectedDeviceInfo.accountId);
        if(clientSessionInformation == null) {
            this.sendMsg_SC_C_CLIENT_VALIDATION_FAILED(client);
            client.forceDisconnectTimeoutS = RegisteredNetClientsSessionValidator.MAX_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            
            // Device not found on the server.
            GlobalAccessLogger.getLog().securityWarning("Client tried to connect with a inexistent device id.");

            return ;
        }

        // Generate the on_server test result;
        let server_sessionTestResultString = md5(clientSessionInformation.key + client.sessionTestString);

        // Initialize encription.
        let encryptionKeyString: string = md5(server_sessionTestResultString);
        let keyStringBuffer: Buffer = Buffer.from(encryptionKeyString, "utf8");
        let key: Uint8Array = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 32);
        let iv: Uint8Array = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 8);

        client.getCon().encryptionCipher = CryptoUtil.newEncryptCipher(
            CryptoUtil.Algorithms_Salsa20, 
            key, iv
        );

        // Initialize encription.
        let decryptionKeyString: string = server_sessionTestResultString;
        keyStringBuffer = Buffer.from(decryptionKeyString, "utf8");
        key = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 32);
        iv = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 8);

        client.getCon().decryptionCipher = CryptoUtil.newDecryptCipher(
            CryptoUtil.Algorithms_Salsa20, 
            key, iv
        );

        // Read and decrypt the client session test result.
        let eClientSessionTestResultBase64: string = msgNetBuffer.readString();
        let clientSessionTestResultString: string = client.getCon().decryptionCipher.decryptBase64ToString(eClientSessionTestResultBase64);
        if(clientSessionTestResultString.length != RegisteredNetClientsSessionValidator.SESSION_TEST_STRING_LENGTH) {
            this.sendMsg_SC_C_CLIENT_VALIDATION_FAILED(client);
            client.forceDisconnectTimeoutS = RegisteredNetClientsSessionValidator.MAX_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            
            GlobalAccessLogger.getLog().securityWarning("Client session test failed.");

            return ;
        }

        // Check if the client result is corect.
        if(server_sessionTestResultString != clientSessionTestResultString) {
            this.sendMsg_SC_C_CLIENT_VALIDATION_FAILED(client);
            client.forceDisconnectTimeoutS = RegisteredNetClientsSessionValidator.MAX_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;

            GlobalAccessLogger.getLog().securityWarning("Client session test failed.");

            return ;
        }

        client.posValidationAccountId = clientSessionInformation.accountId;
        
        this.sendMsg_SC_C_SET_SESSION_STATUS_VALID(client);
    }

  private processMsg_CS_R_SET_SESSION_STATUS_VALID_OK(client: UnvalidatedClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): void {
    GlobalAccessLogger.getLog().debug('Client session validation. processMsg_CS_R_SET_SESSION_STATUS_VALID_OK.');

    if(client.getCurrentCommandNetCode() != NetCodes_Server_UnvalidatedClient.SC_C_SET_SESSION_STATUS_VALID) {
      throw new Error('Security warning.');
    }
    client.clearCurrentCommandNetCode();

    // Close the connection.
    client.markForRemoval = true;
    
    // Extract the con and call the connection validation success.
    let con: GeneralTcpConnection = this.extractClientCon(client);

    this.onValidationSuccess(client.posValidationAccountId, con);
  }

  protected onValidationSuccess(accountId: number, con: GeneralTcpConnection): void {
    if(this.cbValidationSuccess != null) {
      this.cbValidationSuccess(accountId, con);
    }
  }

  private processMsg_CS_OR_CLIENT_VALIDATION_FAILED_OK(client: UnvalidatedClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): void {
    GlobalAccessLogger.getLog().debug('Client session validation. processMsg_CS_OR_CLIENT_VALIDATION_FAILED_OK.');

    if(client.getCurrentCommandNetCode() != NetCodes_Server_UnvalidatedClient.SC_C_CLIENT_VALIDATION_FAILED) {
      throw new Error('Security warning.');
    }
    client.clearCurrentCommandNetCode();

    // Close the connection.
    client.markForRemoval = true;
  }

}
