import { ClientAccountsInteractorBase } from "./ClientAccountsInteractorBase";

import { IAppLocal } from "./../app/IAppLocal";

import { TcpServer } from "base/common/net/TcpServer";
import { NetClientsHolder } from "base/common/client/net/holder/NetClientsHolder";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { IpPort } from "base/net/util/IpPort";
import { ClientAccountsNetClient } from "./util/ClientAccountsNetClient";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { NetUtil } from "base/common/net/util/NetUtil";
import { CryptoUtil } from "base/crypto/util/CryptoUtil";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import * as md5 from "md5"; 
import { LogicError } from "base/exception/LogicError";
import { ConnectedClientDeviceInfo } from "squareheads/clientdevicesmanager/util/ConnectedClientDeviceInfo";
import { ClientSessionInfo } from "squareheads/clientsessionsmanager/util/ClientSessionInfo";
import { CustomClientAccInfo_SocialUserId_Nickname } from "squareheads/clientaccountsmanager/util/CustomClientAccInfo_SocialUserId_Nickname";
import * as net from "net";
import { GlobalAccessLogger } from "base/log/GlobalAccessLogger";

class NetCodes_AccountsInteractor_Client {

  // ENCRYPTION: Server -> Client: AES(connectedDeviceKey)
  // ENCRYPTION: Client -> Server: AES(connectedDeviceKey + deviceTestResultString)

  public static readonly SC_C_SEND_ACCOUNT_STATUS: number = 100010;
  public static readonly CS_R_SEND_ACCOUNT_STATUS_NO_ACCOUNT: number = 100011;
  public static readonly CS_R_SEND_ACCOUNT_STATUS_OK_AND_DEVICE_CONNECTED: number = 100012;

  public static readonly SC_C_SET_NEW_CONNECTED_DEVICE_INFO: number = 100020;
  // connectedDeviceId: number;
  // connectedDeviceKey: string;
  public static readonly CS_R_SET_NEW_CONNECTED_DEVICE_INFO_OK: number = 100021;
  
  public static readonly SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT: number = 100030;
  // deviceTestString: string;
  public static readonly CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK: number = 100031;
  // connectedDeviceId: number;
  // eDeviceTestResultString: string; (length = md5.length); =MD5(connectedDeviceKey + deviceTestString)
  
  public static readonly SC_C_SET_SESSION_ID: number = 100040;
  // eSessionId: number;
  // eSessionKey: string;
  // eSocialUserId: number;
  // eNickname: string;
  public static readonly CS_OR_SET_SESSION_ID_OK: number = 100041;
  // This is a optional response. Can be ignored if the net con will be closed right after.

  public static readonly SC_C_ACCOUNT_INTERACTION_FAILED: number = 100050;
  public static readonly CS_OR_ACCOUNT_INTERACTION_FAILED_OK: number = 100051;
  // This is a optional response. Can be ignored if the net con will be closed right after.

}

export class ClientAccountsInteractor extends ClientAccountsInteractorBase {

    private static MAX_ACCOUNT_HANDLING_DURATION_S: number = 15;
    private static MAX_ACCOUNT_POST_VALIDATION_DELAYED_CLOSE_DURATION_S: number = 5;

  private static DEVICE_TEST_STRING_LENGTH: number = 32;
  
  private netClientsListener: TcpServer = null;
  private netClientsHolder: NetClientsHolder<ClientAccountsNetClient> = null;

  public constructor(app: IAppLocal) {
    super(app);
    
    //void
  }

  protected create_framework(): void {
    super.create_framework();
    
    this.netClientsListener = new TcpServer(
      new IpPort(
        '127.0.0.1',
        0
      ),

      this.onNewSocket.bind(this),
      this.onListenerNetError.bind(this),
      this.onListenerLogicError.bind(this),
      null
    );
    this.netClientsListener.create();
    
    this.addComponent(this.netClientsHolder = new NetClientsHolder<ClientAccountsNetClient>(
      this.onNewClient.bind(this),
      this.onClientRemoved_pre.bind(this),
      this.onProcessClientMsg.bind(this)
    ));
  }

  private onNewSocket(target: TcpServer, socket: net.Socket): void {
    let con: GeneralTcpConnection = new GeneralTcpConnection(socket);
    let netClient: ClientAccountsNetClient = new ClientAccountsNetClient(con);
    this.netClientsHolder.addClient(netClient);
  }

  private onListenerNetError(target: TcpServer, error: Error): void {
    this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');

    this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
  }

  private onListenerLogicError(target: TcpServer, error: Error): void {
    this.getApp().getLog().error('TcpServer. Error. Maybe the port is already in use.');

    this.getApp().getErrorHandler().error('TcpServer. Error. Maybe the port is already in use.');
  }

    private onNewClient(client: ClientAccountsNetClient): void {
        client.forceDisconnectTimeoutS = ClientAccountsInteractor.MAX_ACCOUNT_HANDLING_DURATION_S;

        // Request the client account information.
        this.sendMsg_SC_C_SEND_ACCOUNT_STATUS(client);
        this.getApp().getLog().debug('Client validation. Connection occured. Starting validation process.');
    }

  private onClientRemoved_pre(client: ClientAccountsNetClient): void {
    //void
  }

  private onProcessClientMsg(client: ClientAccountsNetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    switch (msgType) {
      case NetCodes_AccountsInteractor_Client.CS_R_SEND_ACCOUNT_STATUS_NO_ACCOUNT:
        if(client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_SEND_ACCOUNT_STATUS) {
          throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();

        // Register a new account.
        let accountId: number = this.getApp().getSquareHeads().getClientAccountsManager().createNewSquareHeadsAccount();

        // Set a new connected device.
        let connectedDeviceInfo: ConnectedClientDeviceInfo = this.getApp().getSquareHeads().getClientDevicesManager().connectNewDeviceToAccount(accountId);
        
        // Send the new connected device id.
        this.sendMsg_SC_C_SET_NEW_CONNECTED_DEVICE_INFO(
          client,
          connectedDeviceInfo.id, 
          connectedDeviceInfo.key
        );
        this.getApp().getLog().debug('Client validation. New account created.');
        
        break;
      case NetCodes_AccountsInteractor_Client.CS_R_SEND_ACCOUNT_STATUS_OK_AND_DEVICE_CONNECTED:
        if(client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_SEND_ACCOUNT_STATUS) {
          throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();

        // 32 random hex characters.
        client.deviceTestString = CryptoUtil.randomHexString(ClientAccountsInteractor.DEVICE_TEST_STRING_LENGTH);

        // Request the connected device id and test result.
        this.sendMsg_SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT(
          client,
          client.deviceTestString
        );
        this.getApp().getLog().debug('Client validation. Testing existent connected device.');
        
        break;
      case NetCodes_AccountsInteractor_Client.CS_R_SET_NEW_CONNECTED_DEVICE_INFO_OK:
        if(client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_SET_NEW_CONNECTED_DEVICE_INFO) {
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
        if(client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_SET_SESSION_ID) {
          throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();

        // Close the connection.
        client.markForRemoval = true;
        
        break;
      case NetCodes_AccountsInteractor_Client.CS_OR_ACCOUNT_INTERACTION_FAILED_OK:
        if(client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_ACCOUNT_INTERACTION_FAILED) {
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

    private processMsg_CS_R_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT_OK(client: ClientAccountsNetClient, msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): void {
        if(client.getCurrentCommandNetCode() != NetCodes_AccountsInteractor_Client.SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT) {
            this.getApp().getLog().security("Client failed device validation.");

            throw new Error('Security warning.');
        }
        client.clearCurrentCommandNetCode();

        let connectedDeviceId: number = msgNetBuffer.readInt();

        // Check if there is a device with the specified id.
        // Also get the device key.
        let serverConnectedDeviceInfo: ConnectedClientDeviceInfo = this.getApp().getSquareHeads().getClientDevicesManager().getConnectedDeviceInfoFromDeviceId(connectedDeviceId);
        if(serverConnectedDeviceInfo == null) {
            this.sendMsg_SC_C_ACCOUNT_INTERACTION_FAILED(client);
            client.forceDisconnectTimeoutS = ClientAccountsInteractor.MAX_ACCOUNT_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;

            // Device not found on the server.
            this.getApp().getLog().securityWarning("Client tried to connect with a inexistent device id.");

            return ;
        }

        let serverDeviceTestResult: string = md5(serverConnectedDeviceInfo.key + client.deviceTestString);

        // Initialize encription.
        let decryptionKeyString: string = serverDeviceTestResult;
        let keyStringBuffer: Buffer = Buffer.from(decryptionKeyString, "utf8");
        let key: Uint8Array = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 32);
        let iv: Uint8Array = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 8);

        client.getCon().decryptionCipher = CryptoUtil.newDecryptCipher(
            CryptoUtil.Algorithms_Salsa20, 
            key, iv
        );

        let eClientDeviceTestResultBase64: string = msgNetBuffer.readString();
        let clientDeviceTestResultString: string = client.getCon().decryptionCipher.decryptBase64ToString(eClientDeviceTestResultBase64);
        if(clientDeviceTestResultString.length != ClientAccountsInteractor.DEVICE_TEST_STRING_LENGTH) {
            this.sendMsg_SC_C_ACCOUNT_INTERACTION_FAILED(client);
            client.forceDisconnectTimeoutS = ClientAccountsInteractor.MAX_ACCOUNT_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;

            this.getApp().getLog().securityWarning("Client failed device test length.");

            return ;
        }

        if(serverDeviceTestResult != clientDeviceTestResultString) {
            this.sendMsg_SC_C_ACCOUNT_INTERACTION_FAILED(client);
            client.forceDisconnectTimeoutS = ClientAccountsInteractor.MAX_ACCOUNT_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
            
            this.getApp().getLog().securityWarning("Client failed device test.");

            return ;
        }

        // Client passed device test. Get valid session information and  send it.

        // Initialize encription.
        let encryptionKeyString: string = md5(serverDeviceTestResult);
        keyStringBuffer = Buffer.from(encryptionKeyString, "utf8");
        key = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 32);
        iv = new Uint8Array(keyStringBuffer.buffer, keyStringBuffer.byteOffset + 0, 8);

        client.getCon().encryptionCipher = CryptoUtil.newEncryptCipher(
            CryptoUtil.Algorithms_Salsa20, 
            key, iv
        );

        let accountId: number = serverConnectedDeviceInfo.accountId;

        // Get existent or create a new session id and key.
        let sessionInformation: ClientSessionInfo = this.getApp().getSquareHeads().getClientSessionsManager().getOrCreateClientSessionWithAccId(accountId);
        let userInfo_socialUserId_nickname: CustomClientAccInfo_SocialUserId_Nickname = this.getApp().getSquareHeads().getClientAccountsManager().getUserCustomDataFromAccId_socialUserId_nickanme(accountId);

        // Response is valid. Send a new ession id.
        this.sendMsg_SC_C_SET_SESSION_ID(
            client,
            sessionInformation.id, 
            sessionInformation.key, 
            userInfo_socialUserId_nickname.socialUserId,
            userInfo_socialUserId_nickname.nickname
        );
        this.getApp().getLog().debug('Client validation. Ok. Sent client session.');
    }

  private sendMsg_SC_C_SEND_ACCOUNT_STATUS(client: ClientAccountsNetClient): void {
    client.setCurrentCommandNetCode(NetCodes_AccountsInteractor_Client.SC_C_SEND_ACCOUNT_STATUS);
    try {
      NetUtil.sendSimpleNetMsg(client.getCon(), NetCodes_AccountsInteractor_Client.SC_C_SEND_ACCOUNT_STATUS);
    } catch (err) {
      throw err;
    }
  }

  private sendMsg_SC_C_SET_NEW_CONNECTED_DEVICE_INFO(client: ClientAccountsNetClient, connectedDeviceId: number, connectedDeviceKey: string): void {
    client.setCurrentCommandNetCode(NetCodes_AccountsInteractor_Client.SC_C_SET_NEW_CONNECTED_DEVICE_INFO);
    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4
      + 4
      + NetMsgsBlock.stringInBufferSize(connectedDeviceKey))
      .startMsgWrite()
      .writeInt(NetCodes_AccountsInteractor_Client.SC_C_SET_NEW_CONNECTED_DEVICE_INFO)
      .writeInt(connectedDeviceId)
      .writeString(connectedDeviceKey)
      .endMsgWrite()
    );
  }

  private sendMsg_SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT(client: ClientAccountsNetClient, deviceTestString: string): void {
        client.setCurrentCommandNetCode(NetCodes_AccountsInteractor_Client.SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT);
        client.getCon().sendBlock(new NetMsgsBlock(1, 
            4
            + NetMsgsBlock.stringInBufferSize(deviceTestString))
            .startMsgWrite()
            .writeInt(NetCodes_AccountsInteractor_Client.SC_C_SEND_CONNECTED_DEVICE_ID_AND_TEST_RESULT)
            .writeString(deviceTestString)
            .endMsgWrite()
        );
  }

  private sendMsg_SC_C_SET_SESSION_ID(client: ClientAccountsNetClient, sessionId: number, sessionKey: string, socialUserId: number, nickname: string): void {
    if(client.getCon().encryptionCipher == null) {
      throw new LogicError("ClientAccountsInteractor.sendMsg_SC_C_SET_SESSION_ID.1.")
    }

    let eSessionId: Buffer = client.getCon().encryptionCipher.encryptIntToBuffer(sessionId);
    let eSessionKey: string = client.getCon().encryptionCipher.encryptStringToBase64(sessionKey);
    let eSocialUserId: Buffer = client.getCon().encryptionCipher.encryptIntToBuffer(socialUserId);
    let eNickname: string = client.getCon().encryptionCipher.encryptStringToBase64(nickname);

    GlobalAccessLogger.getLog().debug("Client session generated. sessionKey: " + sessionKey + ", eSessionKey: " + eSessionKey + ", nickname: " + nickname + ", eNickname: " + eNickname + ", sessionId: " + sessionId + ", socialUserId: " + socialUserId);

    client.setCurrentCommandNetCode(NetCodes_AccountsInteractor_Client.SC_C_SET_SESSION_ID);
    client.getCon().sendBlock(new NetMsgsBlock(1, 
      4
      + eSessionId.length
      + NetMsgsBlock.stringInBufferSize(eSessionKey)
      + eSocialUserId.length
      + NetMsgsBlock.stringInBufferSize(eNickname))
      .startMsgWrite()
      .writeInt(NetCodes_AccountsInteractor_Client.SC_C_SET_SESSION_ID)
      .writeBuffer(eSessionId)
      .writeString(eSessionKey)
      .writeBuffer(eSocialUserId)
      .writeString(eNickname)
      .endMsgWrite()
    );

    // Connection finished. start a timeout.
    client.forceDisconnectTimeoutS = ClientAccountsInteractor.MAX_ACCOUNT_POST_VALIDATION_DELAYED_CLOSE_DURATION_S;
  }

  private sendMsg_SC_C_ACCOUNT_INTERACTION_FAILED(client: ClientAccountsNetClient): void {
    client.setCurrentCommandNetCode(NetCodes_AccountsInteractor_Client.SC_C_ACCOUNT_INTERACTION_FAILED);
    try {
      NetUtil.sendSimpleNetMsg(client.getCon(), NetCodes_AccountsInteractor_Client.SC_C_ACCOUNT_INTERACTION_FAILED);
    } catch (err) {
      throw err;
    }
  }

  public getClientsListenerIpPort(): IpPort {
    if(this.netClientsListener != null) {
      let ret: IpPort = this.netClientsListener.getIpPort();
      
      return ret;
    }
    
    return null;
  }

}
