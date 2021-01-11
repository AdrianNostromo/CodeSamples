import { Netcut_SC_ServerManager_To_Component } from "./Netcut_SC_ServerManager_To_Component";

import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { StreamBuffer } from "base/buffer/StreamBuffer";

import { NetCodes_SC_ServerManager_To_NodeController } from "servermanager/netcode/NetCodes_SC_ServerManager_To_NodeController";
import { NetCodes_CS_NodeController_To_ServerManager } from "servermanager/netcode/NetCodes_CS_NodeController_To_ServerManager";
import { IAppLocal } from "servermanager/app/IAppLocal";

export class Netcut_SC_ServerManager_To_NodeController extends Netcut_SC_ServerManager_To_Component {

  public constructor(app: IAppLocal, con: GeneralTcpConnection, componentType: ComponentType) {
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
    
    let componentType: ComponentType;
    let componentTypeID: number;

    switch (msgType) {
      case NetCodes_CS_NodeController_To_ServerManager.RESPONSE_CREATE_COMPONENT_OK:
        // Read the request data.
        try {
          componentTypeID = msgNetBuffer.readInt();
        }catch(err) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        componentType = ComponentType.fromID(componentTypeID);
        if(!componentType) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        //void
        
        break;
      case NetCodes_CS_NodeController_To_ServerManager.RESPONSE_CREATE_COMPONENT_ERROR:
        // Read the request data.
        try {
          componentTypeID = msgNetBuffer.readInt();
        }catch(err) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        componentType = ComponentType.fromID(componentTypeID);
        if(!componentType) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        this.getApp().getLog().error('Component creation error occured.');
        
        this.getApp().getErrorHandler().error('Component creation error occured.');
        
        break;
      default:
        return false;
    }
    
    return true;
  }
  
  private sendMsg_REQUEST_CREATE_COMPONENT(componentType: ComponentType): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_SC_ServerManager_To_NodeController.REQUEST_CREATE_COMPONENT);
      netBlock.writeInt(componentType.id);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
  public requestCreateComponent(componentType: ComponentType): void {
    this.sendMsg_REQUEST_CREATE_COMPONENT(componentType);
  }
  
}
