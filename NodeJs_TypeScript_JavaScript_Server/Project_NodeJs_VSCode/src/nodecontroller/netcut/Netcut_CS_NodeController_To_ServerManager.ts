import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";

import { IAppLocal } from "./../app/IAppLocal";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";
import { StreamBuffer } from "base/buffer/StreamBuffer";

import { NetCodes_SC_ServerManager_To_NodeController } from "servermanager/netcode/NetCodes_SC_ServerManager_To_NodeController";
import { NetCodes_CS_NodeController_To_ServerManager } from "servermanager/netcode/NetCodes_CS_NodeController_To_ServerManager";
import { Netcut_CS_Component_To_ServerManager } from "base/servermanager/netcut/Netcut_CS_Component_To_ServerManager";

export class Netcut_CS_NodeController_To_ServerManager extends Netcut_CS_Component_To_ServerManager {

    private _app: IAppLocal;

    public constructor(app: IAppLocal, con: GeneralTcpConnection, componentType: ComponentType) {
        super(app, con, componentType);

        this._app = app;
    }
  
    public getApp(): IAppLocal {
        return this._app;
    }

    protected create_framework(): void {
        super.create_framework();
        
        //void
    }
  
  protected processMsg(msgLen: number, msgType: number, msgNetBuffer: StreamBuffer): boolean {
    if(super.processMsg(msgLen, msgType, msgNetBuffer)) {
      return true;
    }
    
    switch (msgType) {
      case NetCodes_SC_ServerManager_To_NodeController.REQUEST_CREATE_COMPONENT:
        // Read the request data.
        let componentTypeID: number;
        try {
          componentTypeID = msgNetBuffer.readInt();
        }catch(err) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        let componentType: ComponentType = ComponentType.fromID(componentTypeID);
        if(!componentType) {
          this.getApp().getLog().error('ServerComponentValidator. Logic error.');

          return false;
        }
        
        let ret: boolean = this.getApp().getNodeController().componentsProcessHandler.createNewComponent(componentType);
        if(!ret) {
          this.sendMsg_RESPONSE_CREATE_COMPONENT_ERROR(componentType);
        }else {
          this.sendMsg_RESPONSE_CREATE_COMPONENT_OK(componentType);
        }
        
        break;
      default:
        return false;
    }
    
    return true;
  }
  
  private sendMsg_RESPONSE_CREATE_COMPONENT_OK(componentType: ComponentType): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_CS_NodeController_To_ServerManager.RESPONSE_CREATE_COMPONENT_OK);
      netBlock.writeInt(componentType.id);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
  private sendMsg_RESPONSE_CREATE_COMPONENT_ERROR(componentType: ComponentType): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4 + 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_CS_NodeController_To_ServerManager.RESPONSE_CREATE_COMPONENT_ERROR);
      netBlock.writeInt(componentType.id);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
}
