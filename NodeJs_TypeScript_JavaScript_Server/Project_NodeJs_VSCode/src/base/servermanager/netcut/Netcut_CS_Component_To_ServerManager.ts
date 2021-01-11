import { Netcut_CS_ClientComponent_To_ServerComponent } from "base/common/net/netcut/Netcut_CS_ClientComponent_To_ServerComponent";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";

import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { StreamBuffer } from "base/buffer/StreamBuffer";
import { ComponentStatus } from "base/common/util/ComponentStatus";
import { ServerStatus } from "base/common/server/ServerStatus";
import { IpPort } from "base/net/util/IpPort";

import { NetCodes_SC_ServerManager_To_ClientComponent } from "servermanager/netcode/NetCodes_SC_ServerManager_To_ClientComponent";
import { NetCodes_CS_ClientComponent_To_ServerManager } from "servermanager/netcode/NetCodes_CS_ClientComponent_To_ServerManager";
import { IApp } from "base/app/IApp";

export class Netcut_CS_Component_To_ServerManager extends Netcut_CS_ClientComponent_To_ServerComponent {

  public constructor(app: IApp, con: GeneralTcpConnection, componentType: ComponentType) {
    super(app, con, componentType);
    
    //void
  }
  
  protected create_framework(): void {
    super.create_framework();
    
    //void
  }
  
  protected processMsg(msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(super.processMsg(msgLen, msgType, msgNetBuffer)) {
      return true;
    }

    let targetComponentTypeID: number;
    let targetComponentType: ComponentType;

    switch (msgType) {
      case NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_SYNC_COMPONENT_LISTENER_IP_PORT_OK:
        //void
        
        break;
      case NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_OK:
        // Read the data.
        let ip: string;
        let port: number;
        try {
          targetComponentTypeID = msgNetBuffer.readInt();
          ip = msgNetBuffer.readString();
          port = msgNetBuffer.readInt();
        }catch(err) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        targetComponentType = ComponentType.fromID(targetComponentTypeID);
        if(!targetComponentType) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        // Save the component listener information.
        let ipPort: IpPort = new IpPort(ip, port);
        
        // Push the data to the app.getComponentDependenciesHandler().
        if(!this.getApp().getComponentDependenciesHandler().addDependencyComponentListenerIpPort(targetComponentType, ipPort)) {
          // Not error because debug at a certain step on the server Manager may cause this request to be sent multiple times before the response is received.
          //this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          //return false;
        }
        
        break;
      case NetCodes_SC_ServerManager_To_ClientComponent.RESPONSE_COMPONENT_LISTENER_IP_PORT_FROM_TYPE_COMPONENT_NOT_CONNECTED:
        // Read the data.
        try {
          targetComponentTypeID = msgNetBuffer.readInt();
        }catch(err) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        targetComponentType = ComponentType.fromID(targetComponentTypeID);
        if(!targetComponentType) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        //void
        
        break;
      case NetCodes_SC_ServerManager_To_ClientComponent.REQUEST_SET_SERVER_STATUS:
        // Read the data.
        let serverStatusId: number;
        try {
          serverStatusId = msgNetBuffer.readInt();
        }catch(err) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        let serverStatus: ServerStatus = ServerStatus.fromID(serverStatusId);
        if(!serverStatus) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        // Save the value.
        this.getApp().setServerStatus(serverStatus);
        
        // Send response.
        if(!this.sendMsg_RESPONSE_SET_SERVER_STATUS_OK()) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');
          
          return false;
        }
        
        break;
      default:
        return false;
    }
    
    return true;
  }
  
  protected syncComponentStatus(componentStatus: ComponentStatus): void {
    if(componentStatus == ComponentStatus.ONLINE_FOR_COMPONENTS && this.getApp().isListenerEnabled()) {
      let ipPort: IpPort = this.getApp().getListenerIpPort();
      if(!ipPort) {
        this.handleConError();
        
        return ;
      }
      
      if(!this.sendMsg_REQUEST_SYNC_COMPONENT_LISTENER_IP_PORT(ipPort)) {
        this.handleConError();
        
        return ;
      }
    }
    
    super.syncComponentStatus(componentStatus);
  }
  
  private sendMsg_REQUEST_SYNC_COMPONENT_LISTENER_IP_PORT(ipPort: IpPort): boolean {
    if(!ipPort) {
      return false;
    }
    
    let ipStringLen: number = Buffer.byteLength(ipPort.ip, 'utf8');
    
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + (2 + ipStringLen) + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_CS_ClientComponent_To_ServerManager.REQUEST_SYNC_COMPONENT_LISTENER_IP_PORT);
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
  
  private sendMsg_REQUEST_COMPONENT_LISTENER_IP_PORT_FROM_TYPE(componentType: ComponentType): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_CS_ClientComponent_To_ServerManager.REQUEST_COMPONENT_LISTENER_IP_PORT_FROM_TYPE);
      netBlock.writeInt(componentType.id);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
  private sendMsg_RESPONSE_SET_SERVER_STATUS_OK(): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_CS_ClientComponent_To_ServerManager.RESPONSE_SET_SERVER_STATUS_OK);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
  public requestComponentListenerIpPort(componentType: ComponentType):boolean  {
    if(!this.sendMsg_REQUEST_COMPONENT_LISTENER_IP_PORT_FROM_TYPE(componentType)) {
      this.handleConError();
      
      return false;
    }
    
    return true;
  }
  
}
