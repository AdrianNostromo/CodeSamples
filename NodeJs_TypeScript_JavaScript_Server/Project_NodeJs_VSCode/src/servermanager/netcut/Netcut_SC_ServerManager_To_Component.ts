import { Netcut_SC_ServerComponent_To_ClientComponent } from "base/common/net/netcut/Netcut_SC_ServerComponent_To_ClientComponent";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";

import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { IpPort } from "base/net/util/IpPort";
import { ServerStatus } from "base/common/server/ServerStatus";

import { NetCodes_SC_ServerManager_To_ClientComponent } from "servermanager/netcode/NetCodes_SC_ServerManager_To_ClientComponent";
import { NetCodes_CS_ClientComponent_To_ServerManager } from "servermanager/netcode/NetCodes_CS_ClientComponent_To_ServerManager";
import { IAppLocal } from "servermanager/app/IAppLocal";

export class Netcut_SC_ServerManager_To_Component extends Netcut_SC_ServerComponent_To_ClientComponent {

  private _remoteComponentListenerIpPort: IpPort = null;
  
  private _sd_serverStatus: ServerStatus = ServerStatus.OFFLINE;

  public constructor(app: IAppLocal, con: GeneralTcpConnection, componentType: ComponentType) {
    super(app, con, componentType);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    //void
  }
  
  public logicLoop(deltaS: number, deltaMS: number): void {
    super.logicLoop(deltaS, deltaMS);
    
    if(this._sd_serverStatus != this.getApp().getServerStatus()) {
      this._sd_serverStatus = this.getApp().getServerStatus();
      
      if(!this.sendMsg_REQUEST_SET_SERVER_STATUS(this._sd_serverStatus)) {
        this.handleConError();
        
        return ;
      }
    }
  }
  
  protected processMsg(msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(super.processMsg(msgLen, msgType, msgNetBuffer)) {
      return true;
    }
    
    switch (msgType) {
      case NetCodes_CS_ClientComponent_To_ServerManager.REQUEST_SYNC_COMPONENT_LISTENER_IP_PORT:
        if(this._remoteComponentListenerIpPort) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        let ip: string;
        let port: number;
        try {
          ip = msgNetBuffer.readString();
          port = msgNetBuffer.readInt();
        }catch(err) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        // Save the component listener information.
        this._remoteComponentListenerIpPort = new IpPort(ip, port);
        
        if(!this.sendMsg_RESPONSE_SYNC_COMPONENT_LISTENER_IP_PORT_OK()) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');
          
          return false;
        }
        
        break;
      case NetCodes_CS_ClientComponent_To_ServerManager.REQUEST_COMPONENT_LISTENER_IP_PORT_FROM_TYPE:
        // Read the request data.
        let targetComponentTypeID: number;
        
        try {
          targetComponentTypeID = msgNetBuffer.readInt();
        }catch(err) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        let targetComponentType: ComponentType = ComponentType.fromID(targetComponentTypeID);
        if(!targetComponentType) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        // Pool the response data.
        let targetComponentNetcut: Netcut_SC_ServerManager_To_Component = <Netcut_SC_ServerManager_To_Component>this.getApp().getClientComponentNetcuts().getDefaultOnlineNetcut(targetComponentType);
        if(!targetComponentNetcut) {
          // The component has not connected yet.
          this.sendMsg_RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_COMPONENT_NOT_CONNECTED(targetComponentType);
        }else {
          let componentListenerIpPort: IpPort = targetComponentNetcut.getRemoteComponentListenerIpPort();
          
          // Return the response data.
          this.sendMsg_RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_OK(targetComponentType, componentListenerIpPort);
        }
        
        break;
      case NetCodes_CS_ClientComponent_To_ServerManager.RESPONSE_SET_SERVER_STATUS_OK:
        //void
        
        break;
      default:
        return false;
    }
    
    return true;
  }
  
  private sendMsg_RESPONSE_SYNC_COMPONENT_LISTENER_IP_PORT_OK(): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_SYNC_COMPONENT_LISTENER_IP_PORT_OK);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
  private sendMsg_RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_OK(targetComponentType: ComponentType, ipPort: IpPort): boolean {
    if(!ipPort) {
      return false;
    }
    
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + 4 + (2 + Buffer.byteLength(ipPort.ip, 'utf8')) + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_OK);
      netBlock.writeInt(targetComponentType.id);
      netBlock.writeString(ipPort.ip);
      netBlock.writeInt(ipPort.port);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
  private sendMsg_RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_COMPONENT_NOT_CONNECTED(targetComponentType: ComponentType): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_COMPONENT_NOT_CONNECTED);
      netBlock.writeInt(targetComponentType.id);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
  private sendMsg_REQUEST_SET_SERVER_STATUS(serverStatus: ServerStatus): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_SC_ServerManager_To_ClientComponent.REQUEST_SET_SERVER_STATUS);
      netBlock.writeInt(serverStatus.id);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
  public getRemoteComponentListenerIpPort(): IpPort {
    return this._remoteComponentListenerIpPort;
  }
  
}
