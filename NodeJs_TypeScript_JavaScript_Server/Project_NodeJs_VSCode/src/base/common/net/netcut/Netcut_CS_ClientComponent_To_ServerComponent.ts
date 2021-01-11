import { Netcut_Component_To_Component } from "./Netcut_Component_To_Component";

import { IApp } from "base/app/IApp";
import { GeneralTcpConnection } from "base/common/net/tcp/connection/GeneralTcpConnection";
import { ComponentType } from "base/common/ComponentType";

import { StreamBuffer } from "base/buffer/StreamBuffer";
import { NetMsgsBlock } from "base/common/net/NetMsgsBlock";

import { NetCodes_SC_ServerComponent_To_ClientComponent } from "base/common/net/netcodes/NetCodes_SC_ServerComponent_To_ClientComponent";
import { NetCodes_CS_ClientComponent_To_ServerComponent } from "base/common/net/netcodes/NetCodes_CS_ClientComponent_To_ServerComponent";

export class Netcut_CS_ClientComponent_To_ServerComponent extends Netcut_Component_To_Component {

  public constructor(app: IApp, con: GeneralTcpConnection, componentType: ComponentType) {
    super(app, con, componentType, false);
    
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
    
    switch (msgType) {
      case NetCodes_SC_ServerComponent_To_ClientComponent.REQUEST_PING:
        if(!this.sendMsg_RESPONSE_PING_OK()) {
          this.getApp().getLog().warning('Ping response received from a server connection. Maybe a logic error on the remote component.');

          return false;
        }
        
        break;
      default:
        return false;
    }
    
    return true;
  }
  
  private sendMsg_RESPONSE_PING_OK(): boolean {
    let netBlock: NetMsgsBlock = new NetMsgsBlock(1, 4);
    try {
      netBlock.startMsgWrite();
      // START : msg data;
      netBlock.writeInt(NetCodes_CS_ClientComponent_To_ServerComponent.RESPONSE_PING_OK);
      // END : msg data;
      netBlock.endMsgWrite();
      
      this.sendNetBlock(netBlock);
    } catch (err) {
      return false;
    }
    
    return true;
  }
  
}
